import subprocess
import queue
import time
import threading
import os
from datetime import datetime
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

# ----------------------------
# PATHS
# ----------------------------
BASE_DIR = os.path.abspath(os.path.dirname(__file__))
LEGACY_DIR = os.path.abspath(os.path.join(BASE_DIR, "../legacy_c"))
LEGACY_BIN = os.path.join(LEGACY_DIR, "legacy_system")
ORDERS_FILE = os.path.join(LEGACY_DIR, "orders.txt")

# ----------------------------
# QUEUE + SYNC
# ----------------------------
job_queue = queue.Queue()
event_lock = threading.Lock()
last_event_time = 0


# ----------------------------
# LOG
# ----------------------------
def write_log(message):
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    with open(os.path.join(BASE_DIR, "automation.log"), "a") as log:
        log.write(f"[{timestamp}] {message}\n")


# ----------------------------
# VALIDATION
# ----------------------------
def validate_orders():
    try:
        with open(ORDERS_FILE, "r") as f:
            lines = [l.strip() for l in f if l.strip()]

        for line in lines:
            if not line.startswith("PEDIDO_"):
                return False

        return True

    except Exception as e:
        write_log(f"ERRO ao validar orders: {str(e)}")
        return False


# ----------------------------
# LEGACY EXECUTION
# ----------------------------
def run_legacy_system():
    try:
        write_log("Iniciando sistema legado")

        # valida antes de rodar
        if not validate_orders():
            write_log("ERRO: orders.txt inválido. Execução abortada.")
            return

        result = subprocess.run(
            [LEGACY_BIN, "auto"],
            cwd=LEGACY_DIR,
            text=True,
            capture_output=True
        )

        if result.stdout:
            write_log(f"STDOUT:\n{result.stdout.strip()}")

        if result.stderr:
            write_log(f"STDERR:\n{result.stderr.strip()}")

        if result.returncode != 0:
            write_log(f"ERRO: Sistema falhou com código {result.returncode}")
        else:
            write_log("Execução concluída com sucesso")

    except Exception as e:
        write_log(f"ERRO CRÍTICO: {str(e)}")


# ----------------------------
# WORKER
# ----------------------------
def worker():
    while True:
        job_queue.get()
        try:
            run_legacy_system()
        finally:
            job_queue.task_done()

threading.Thread(target=worker, daemon=True).start()


# ----------------------------
# WATCHDOG
# ----------------------------
class OrderHandler(FileSystemEventHandler):

    def on_modified(self, event):
        global last_event_time

        if os.path.basename(event.src_path) != "orders.txt":
            return

        now = time.time()

        with event_lock:
            if now - last_event_time < 2:
                return
            last_event_time = now

        write_log("Evento enfileirado")
        job_queue.put(True)


# ----------------------------
# MAIN
# ----------------------------
if __name__ == "__main__":
    write_log("Iniciando monitoramento de pedidos")

    observer = Observer()
    observer.schedule(OrderHandler(), path=LEGACY_DIR, recursive=False)

    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        write_log("Encerrando monitoramento")
        observer.stop()

    observer.join()