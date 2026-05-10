import subprocess
import time
from datetime import datetime

from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler


LEGACY_PATH = "../legacy_c/legacy_system"
ORDERS_FILE = "../legacy_c/orders.txt"

last_run = 0

def write_log(message):
    
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    
    with open("automation.log", "a") as log:
        log.write(f"[{timestamp}] {message}\n")
        

def run_legacy_system():
    
    write_log("Inciando sistema legado")
    
    try:
        result = subprocess.run(
            [LEGACY_PATH],
            input="1\n",
            text=True,
            capture_output=True
        )
        
        write_log(f"STDOUT: {result.stdout}")
        write_log(f"STDERR: {result.stderr}")
        
        if result.returncode != 0:
            
            write_log(f"ERRO: Sistema falhou com código {result.returncode}")
            
            return
        
        write_log("Execução concluída com sucesso")
    
    except Exception as e:
        
        write_log(f"ERRO CRÍTICO: {str(e)}")
 
class OrderHandler(FileSystemEventHandler):
    
     def on_modified(self, event):
         
        global last_run
         
        if not event.src_path.endswith("orders.txt"):
             return
        
        now = time.time() 
        
        if now - last_run < 2:
            return
        
        last_run = now
        
        write_log("Alteração detectada em orders.txt")
        
        run_legacy_system()
         

if __name__ == "__main__":
    
    write_log("Iniciando monitoramento de pedidos")
    
    event_handler = OrderHandler()
    
    observer = Observer()
    
    observer.schedule(
        event_handler,
        path="../legacy_c",
        recursive=False
    )
    
    observer.start()
    
    try:
        
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        
        observer.stop()
    
    observer.join()
