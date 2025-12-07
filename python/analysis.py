#!/usr/bin/env python3
"""
Script simple pour tester les algorithmes TSP
Génère juste un fichier CSV avec les résultats
"""

import subprocess
import time

# =============================================================================
# CONFIGURATION
# =============================================================================

INSTANCES = [
    "../test/burma14.tsp",
    "../test/att48.tsp",
    "../test/berlin52.tsp",
]

METHODS = [
    "nn",
    "rw",
    "2optnn",
    "2optrw",
    "ga 10000 100 0.10",
    "gadpx 10000 100 0.10"
]

PROGRAM = "../bin/tsp"
REPETITIONS = 10
OUTPUT_FILE = "resultats_tsp.csv"

# Solutions optimales
OPTIMAL = {
    "burma14": 3323,
    "att48": 10628,
    "berlin52": 7542,
}

# =============================================================================
# CODE
# =============================================================================

def run_test(program, instance, method):
    """Exécute un test et retourne le résultat"""
    cmd = [program, "-f", instance, "-m"] + method.split()
    
    try:
        start = time.time()
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
        elapsed = time.time() - start
        
        output = result.stdout.strip()
        lines = output.split('\n')
        
        # Trouver la ligne de données
        data_line = None
        for line in lines:
            if line and not line.startswith('Instance'):
                data_line = line
                break
        
        if not data_line:
            return None
        
        # Parser: instance ; method ; time ; length ; [tour]
        parts = [p.strip() for p in data_line.split(';')]
        
        if len(parts) < 4:
            return None
        
        return {
            'instance': parts[0],
            'algorithm': parts[1],
            'time_cpu': float(parts[2]),
            'length': int(float(parts[3])),
            'time_real': elapsed
        }
    except:
        return None

def main():
    print("Lancement des tests...")
    print(f"{len(INSTANCES)} instances × {len(METHODS)} algorithmes × {REPETITIONS} répétitions")
    print(f"Total: {len(INSTANCES) * len(METHODS) * REPETITIONS} tests\n")
    
    # Ouvrir le fichier CSV
    with open(OUTPUT_FILE, 'w') as f:
        # En-tête
        f.write("instance,algorithme,repetition,temps_cpu,temps_reel,longueur,optimal,ecart_pct\n")
        
        total = len(INSTANCES) * len(METHODS) * REPETITIONS
        current = 0
        
        for instance in INSTANCES:
            inst_name = instance.split('/')[-1].replace('.tsp', '')
            optimal = OPTIMAL.get(inst_name, None)
            
            for method in METHODS:
                print(f"Testing {inst_name:12s} - {method:20s} ", end='', flush=True)
                
                for rep in range(REPETITIONS):
                    current += 1
                    result = run_test(PROGRAM, instance, method)
                    
                    if result:
                        # Calculer écart
                        if optimal:
                            gap = ((result['length'] - optimal) / optimal) * 100
                        else:
                            gap = None
                        
                        # Écrire dans le CSV
                        f.write(f"{inst_name},{result['algorithm']},{rep+1},")
                        f.write(f"{result['time_cpu']:.6f},{result['time_real']:.6f},")
                        f.write(f"{result['length']},{optimal if optimal else ''},")
                        f.write(f"{gap:.2f}\n" if gap is not None else "\n")
                        
                        print("✓", end='', flush=True)
                    else:
                        print("✗", end='', flush=True)
                
                print(f"  [{current}/{total}]")
    
    print(f"\n✅ Terminé! Résultats dans: {OUTPUT_FILE}")
    print(f"\nVous pouvez maintenant ouvrir {OUTPUT_FILE} dans Excel/LibreOffice\n")

if __name__ == '__main__':
    main()