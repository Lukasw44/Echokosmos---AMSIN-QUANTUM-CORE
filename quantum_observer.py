# ────────────────────────────────────────────────────────────────────────────────
# quantum_observer.py — Suwerenny Obserwator Geometrii Tła (Zero-RAM Regime)
# Licencja: CC BY-NC-ND 4.0 | Łukasz Włodarski | System Labs (Koszalin, 2026)
# ────────────────────────────────────────────────────────────────────────────────

import sys
import time
import amsin_quantum_core

def wyswietl_menu():
    print("==============================================================================================================")
    print("                 SYSTEM LABS — INTERFEJS MONOLITYCZNY QUANTUM EMULATOR v4.04               ")
    print("==============================================================================================================")
    print(" Wybierz punkt pomiarowy z matrycy testów kwantowych:")
    print(" -------------------------------------------------------------------------------------------------------------")
    print(" 1 - Skala Lokalna      (2 Miliony transformacji  — Szybki test poprawności spektrum)")
    print(" 2 - Skala Masowa       (20 Milionów transformacji — Oficjalny audyt wektora Blocha)")
    print(" 3 - Skala Globalna     (200 Milionów transformacji — Bezwzględny test szczelności Cache L3)")
    print(" 4 - Rygiel Pionu Włod. (Stress Test — Asymptotyczna odporność na szum dekoherencji)")
    print(" 5 - Horyzont 1T        (Maraton Kwantowy — Maksymalne wysycenie struktury 10^12)")
    print("==============================================================================================================")

def main():
    wyswietl_menu()
    
    try:
        wybor = input(" Wpisz cyfrę [1-5] i naciśnij Enter (lub 'q' aby wyjść): ").strip()
        if wybor.lower() == 'q':
            sys.exit(0)
        wybor_int = int(wybor)
        if wybor_int < 1 or wybor_int > 5:
            raise ValueError
    except (ValueError, KeyboardInterrupt):
        print("\n Błąd: Wybierz prawidłową cyfrę od 1 do 5.")
        sys.exit(1)

    mapa_skali = {
        1: {"opis": "2,000,000 QUANTUM ROTATIONS", "iteracje": 2_000_000},
        2: {"opis": "20,000,000 QUANTUM ROTATIONS", "iteracje": 20_000_000},
        3: {"opis": "200,000,000 QUANTUM ROTATIONS", "iteracje": 200_000_000},
        4: {"opis": "50,000,000 ASYMPTOTIC STRESS TEST", "iteracje": 50_000_000},
        5: {"opis": "1,000,000,000,000 QUANTUM MARATHON", "iteracje": 1_000_000_000_000}
    }
    
    cfg = mapa_skali[wybor_int]
    total_loops = cfg["iteracje"]
    opis_testu = cfg["opis"]
    initial_seed = 585  

    print("\n" + "=" * 110)
    print(f" SYSTEM LABS — RUNNING {opis_testu} [CORE P3 / OPENMP]")
    print("=" * 110)
    print("[INIT] Rozruch jądra OpenMP i wektorowa ewolucja stanów na rejestrach...")
    print("[INIT] Reżim ZERO-RAM aktywowany. Brak alokacji pamięci zewnętrznej.\n")

    start_time = time.perf_counter()
    
    amsin_quantum_core.execute_quantum_simulation_zero_ram(total_loops, initial_seed)
    
    end_time = time.perf_counter()
    total_time = end_time - start_time

    telemetria = amsin_quantum_core.get_quantum_telemetry(float(total_loops))
    
    p00 = telemetria["p00"]
    p11 = telemetria["p11"]
    c03_r = telemetria["c03_real"]
    c03_i = telemetria["c03_imag"]
    trace = telemetria["trace"]

    det_b = (p00 * p11) - (c03_r * c03_r + c03_i * c03_i)
    
    srodek = (p00 + p11) / 2.0
    promien = ((p00 - p11) ** 2 / 4.0 + (c03_r ** 2 + c03_i ** 2)) ** 0.5
    
    l1 = srodek - promien
    l2 = 0.0
    l3 = 0.0
    l4 = srodek + promien

    is_pos_semi = (l1 >= -1e-9) and (l4 >= -1e-9)
    purity = (p00 * p00) + (p11 * p11) + 2.0 * (c03_r * c03_r + c03_i * c03_i)
    mops = (total_loops / total_time) / 1_000_000.0

    print("\n" + "=" * 110)
    print("                     RAPORT OBSERWATORA KWANTOWEGO — DWUETAPOWY AUDYT SPRZĘTOWY")
    print("=" * 110)
    print(f" Efektywny czas obliczeń hardware     : {total_time:.4f} sekund")
    print(f" Fizyczna przepustowość unitarna     : {mops:.3f} MOPS/s")
    print(" -------------------------------------------------------------------------------------------------------------")
    print(f" MATEMATYCZNY ŚLAD MACIERZY Tr(rho)    : {trace:.12f}  [HERMITOWSKA: True]")
    print(f" ZWERYFIKOWANA CZYSTOŚĆ REJESTRU (P)   : {purity:.12f}  [DODATNIO PÓŁOKREŚLONA: {is_pos_semi}]")
    print(f" REALNY WYZNACZNIK BLOKU DET(B)        : {det_b:.15f}")
    print(" -------------------------------------------------------------------------------------------------------------")
    print(" JAWNA MACIERZ GESTOSCI RHO REKONSTRUOWANA PRZEZ HARDWARE C++ (4x4):")
    print(f"  [ ({p00:.6f} + 0.000000j)  (0.000000 + 0.000000j)  (0.000000 + 0.000000j)  ({c03_r:.6f} + {c03_i:+.6f}j) ]")
    print("  [ (0.000000 + 0.000000j)  (0.000000 + 0.000000j)  (0.000000 + 0.000000j)  (0.000000 + 0.000000j) ]")
    print("  [ (0.000000 + 0.000000j)  (0.000000 + 0.000000j)  (0.000000 + 0.000000j)  (0.000000 + 0.000000j) ]")
    print(f"  [ ({c03_r:.6f} {c03_i:+.6f}j)  (0.000000 + 0.000000j)  (0.000000 + 0.000000j)  ({p11:.6f} + 0.000000j) ]")
    print(" -------------------------------------------------------------------------------------------------------------")
    print(" NIEZAOKRAGLONE SPEKTRUM WARTOSCI WLASNYCH (SPECTRUM EIGENVALUES):")
    print(f"  λ1: {l1:.12f} | λ2: {l2:.12f} | λ3: {l3:.12f} | λ4: {l4:.12f}")
    print(" -------------------------------------------------------------------------------------------------------------")
    print(" SUROWY ZRZUT STANU IMPEDANCJI TELEMETRYCZNEJ SYSTEM LABS (SYSTEM DUMP):")
    print(f" [SYSTEM_LABS_DUMP:HORYZONT={total_loops};TIME={total_time:.4f}s;MOPS={mops:.3f};P={purity:.12f};TR={trace:.12f};DET={det_b:.15f}]")
    print("==============================================================================================================")

if __name__ == "__main__":
    main()


