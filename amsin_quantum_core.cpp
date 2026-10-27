// ────────────────────────────────────────────────────────────────────────────────
// amsin_quantum_core.cpp —  Dynamiczne Sprzężenie Pętli (Reżim 676)
// Licencja: CC BY-NC-ND 4.0 | Łukasz Włodarski | System Labs (Koszalin, 2026)
// ────────────────────────────────────────────────────────────────────────────────

#include <iostream>
#include <cmath>
#include <cstdint>
#include <immintrin.h>
#include <atomic>
#include <chrono>
#include <omp.h>
#include <intrin.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <vector>
#include <complex>

namespace py = pybind11;

constexpr uint64_t BAZA_A0            = 585;
constexpr uint64_t GTS_KROK           = 91;
constexpr uint64_t PHI_44_SCALE       = 4418;
constexpr uint64_t HORYZONT_1170      = 1170;
constexpr uint64_t QUANTUM_GATE_MASK  = 0x5555555555555555ULL;
constexpr int64_t CYKL_HOLO_GIGA      = 630000000LL;

std::atomic<int64_t> g_global_quantum_counter{0};
std::atomic<int64_t> g_holographic_gear_tier{0};
std::atomic<uint64_t> g_z2_safe_mode_popcount{0};
std::atomic<uint64_t> g_zibi_activations{0};
std::atomic<uint64_t> g_ml_exhaust_triggers{0};
std::atomic<uint64_t> g_global_quantum_sq_sum{0};
std::atomic<uint64_t> g_dynamic_tensor_z12_luz{0};
std::atomic<uint64_t> g_dynamic_tensor_z16_uziemienie{0};

std::atomic<double> g_bloch_global_x{0.0};
std::atomic<double> g_bloch_global_y{0.0};
std::atomic<double> g_bloch_global_z{0.0};
std::atomic<double> g_hadamard_global_x{0.0};
std::atomic<double> g_hadamard_global_y{0.0};
std::atomic<double> g_hadamard_global_z{0.0};
std::atomic<double> g_phase_global_x{0.0};
std::atomic<double> g_phase_global_y{0.0};
std::atomic<double> g_phase_global_z{0.0};
std::atomic<double> g_pauliz_global_x{0.0};
std::atomic<double> g_pauliz_global_y{0.0};
std::atomic<double> g_pauliz_global_z{0.0};
std::atomic<double> g_cnot_pop_00{0.0};
std::atomic<double> g_cnot_pop_01{0.0};
std::atomic<double> g_cnot_pop_10{0.0};
std::atomic<double> g_cnot_pop_11{0.0};
std::atomic<double> g_cnot_coh_03_real{0.0};
std::atomic<double> g_cnot_coh_03_imag{0.0};

static inline uint64_t fast_base10_sum_tree_quantum(uint64_t u) {
    uint64_t sum = 0;
    uint64_t hex_sum = (u & 0x0F0F0F0F0F0F0F0FULL) + ((u >> 4) & 0x0F0F0F0F0F0F0F0FULL);
    hex_sum = (hex_sum & 0x00FF00FF00FF00FFULL) + ((hex_sum >> 8) & 0x00FF00FF00FF00FFULL);
    hex_sum = (hex_sum & 0x0000FFFF0000FFFFULL) + ((hex_sum >> 16) & 0x0000FFFF0000FFFFULL);
    sum = (hex_sum & 0x00000000FFFFFFFFULL) + (hex_sum >> 32);
    uint64_t correction = ((u >> 4) & 0x0F0F0F0F0F0F0F0FULL) * 6;
    uint64_t hex_corr = (correction & 0x0F0F0F0F0F0F0F0FULL) + ((correction >> 4) & 0x0F0F0F0F0F0F0F0FULL);
    hex_corr = (hex_corr & 0x00FF00FF00FF00FFULL) + ((hex_corr >> 8) & 0x00FF00FF00FF00FFULL);
    hex_corr = (hex_corr & 0x0000FFFF0000FFFFULL) + ((hex_corr >> 16) & 0x0000FFFF0000FFFFULL);
    uint64_t corr_sum = (hex_corr & 0x00000000FFFFFFFFULL) + (hex_corr >> 32);
    return sum - corr_sum;
}

static inline uint64_t fast_base16_sum_tree_quantum(uint64_t u) {
    u = (u & 0x0F0F0F0F0F0F0F0FULL) + ((u >> 4) & 0x0F0F0F0F0F0F0F0FULL);
    u = (u & 0x00FF00FF00FF00FFULL) + ((u >> 8) & 0x00FF00FF00FF00FFULL);
    u = (u & 0x0000FFFF0000FFFFULL) + ((u >> 16) & 0x0000FFFF0000FFFFULL);
    return (u & 0x00000000FFFFFFFFULL) + (u >> 32);
}

static inline uint64_t fast_base12_sum_tree_quantum(uint64_t u) {
    uint64_t hex_sum = (u & 0x0F0F0F0F0F0F0F0FULL) + ((u >> 4) & 0x0F0F0F0F0F0F0F0FULL);
    hex_sum = (hex_sum & 0x00FF00FF00FF00FFULL) + ((hex_sum >> 8) & 0x00FF00FF00FF00FFULL);
    hex_sum = (hex_sum & 0x0000FFFF0000FFFFULL) + ((hex_sum >> 16) & 0x0000FFFF0000FFFFULL);
    uint64_t sum = (hex_sum & 0x00000000FFFFFFFFULL) + (hex_sum >> 32);
    uint64_t correction = ((u >> 4) & 0x0F0F0F0F0F0F0F0FULL) * 4;
    uint64_t hex_corr = (correction & 0x0F0F0F0F0F0F0F0FULL) + ((correction >> 4) & 0x0F0F0F0F0F0F0F0FULL);
    hex_corr = (hex_corr & 0x00FF00FF00FF00FFULL) + ((hex_corr >> 8) & 0x00FF00FF00FF00FFULL);
    hex_corr = (hex_corr & 0x0000FFFF0000FFFFULL) + ((hex_corr >> 16) & 0x0000FFFF0000FFFFULL);
    uint64_t corr_sum = (hex_corr & 0x00000000FFFFFFFFULL) + (hex_corr >> 32);
    return sum - corr_sum;
}

static inline uint64_t execute_quantum_sq_operator(uint64_t val) {
    uint64_t temp = val;
    uint64_t sq_bit_sum = 0;
    while (temp > 0) {
        uint64_t digit_b4 = temp % 4ULL;
        if (digit_b4 == 1ULL || digit_b4 == 3ULL) {
            sq_bit_sum += 1ULL;
        } else if (digit_b4 == 2ULL) {
            sq_bit_sum += 2ULL;
        }
        temp /= 4ULL;
    }
    return sq_bit_sum;
}

static inline uint64_t execute_p3_takt_pure(uint64_t index_n, uint64_t& last_b) {
    uint64_t a_n = BAZA_A0 + GTS_KROK * index_n;
    uint64_t s_last_b = fast_base10_sum_tree_quantum(last_b);

    if (s_last_b >= 22ULL) {
        g_ml_exhaust_triggers.fetch_add(1, std::memory_order_relaxed);
        int64_t b_next = static_cast<int64_t>(last_b) + (91LL * (static_cast<int64_t>(s_last_b) - 17LL)) + 4418LL - 315LL;
        b_next = b_next % static_cast<int64_t>(HORYZONT_1170);
        if (b_next < 0) b_next += HORYZONT_1170;
        last_b = static_cast<uint64_t>(b_next);
    } else {
        last_b = (last_b + 91ULL * s_last_b + 4418ULL) % HORYZONT_1170;
    }

    int64_t gik_delta = static_cast<int64_t>(last_b) - static_cast<int64_t>(a_n);
    uint64_t gik_n = std::abs(gik_delta) % 585ULL;

    if (gik_n > 44ULL) {
        g_zibi_activations.fetch_add(1, std::memory_order_relaxed);
        last_b ^= (gik_n << 2);
    }

    uint64_t sum_z12 = fast_base12_sum_tree_quantum(last_b);
    if ((sum_z12 % 11ULL) == 2ULL) {
        g_dynamic_tensor_z12_luz.fetch_add(1, std::memory_order_relaxed);
    }

    uint64_t sum_z16 = fast_base16_sum_tree_quantum(last_b);
    if ((sum_z16 % 15ULL) == 0ULL) {
        g_dynamic_tensor_z16_uziemienie.fetch_add(1, std::memory_order_relaxed);
    }

    uint64_t collatza_node = last_b;
    if (collatza_node == 65 || (collatza_node % 13ULL == 0)) {
        uint64_t is_odd = collatza_node & 1ULL;
        collatza_node = (is_odd * (3ULL * collatza_node + 1ULL)) + ((1ULL - is_odd) * (collatza_node >> 1ULL));
        if ((collatza_node % 9ULL) == 7ULL) {
            collatza_node += 7ULL; 
        }
    }

    if (collatza_node == 1260ULL) {
        g_ml_exhaust_triggers.fetch_add(1000, std::memory_order_relaxed);
    }

    uint64_t s2_popcount_raw = __popcnt64(collatza_node);
    uint64_t current_x = (collatza_node % 22ULL); 
    uint64_t current_y = (collatza_node % 630ULL); 
    uint64_t current_z = 34ULL + (s2_popcount_raw % 13ULL);

    uint64_t t_sum = current_x + current_y + current_z;
    if (t_sum % 9ULL == 7ULL) {
        current_y += 1ULL;
    }

    uint64_t rotated_stream = (current_x ^ current_y ^ current_z) ^ QUANTUM_GATE_MASK;
    uint64_t nieparzyste    = rotated_stream & 0x5555555555555555ULL;
    uint64_t parzyste       = (rotated_stream >> 1) & 0x5555555555555555ULL;

    return __popcnt64(~parzyste & ~nieparzyste) + __popcnt64(parzyste & nieparzyste);
}

uint64_t execute_quantum_simulation_zero_ram(int64_t total_loops, uint64_t initial_seed) {
    py::gil_scoped_release release;

    g_global_quantum_counter.store(0, std::memory_order_relaxed);
    g_holographic_gear_tier.store(0, std::memory_order_relaxed);
    g_z2_safe_mode_popcount.store(0, std::memory_order_relaxed);
    g_global_quantum_sq_sum.store(0, std::memory_order_relaxed);
    g_zibi_activations.store(0, std::memory_order_relaxed);
    g_ml_exhaust_triggers.store(0, std::memory_order_relaxed);
    g_dynamic_tensor_z12_luz.store(0, std::memory_order_relaxed);
    g_dynamic_tensor_z16_uziemienie.store(0, std::memory_order_relaxed);

    g_bloch_global_x.store(0.0, std::memory_order_relaxed);
    g_bloch_global_y.store(0.0, std::memory_order_relaxed);
    g_bloch_global_z.store(0.0, std::memory_order_relaxed);
    g_hadamard_global_x.store(0.0, std::memory_order_relaxed);
    g_hadamard_global_y.store(0.0, std::memory_order_relaxed);
    g_hadamard_global_z.store(0.0, std::memory_order_relaxed);
    g_phase_global_x.store(0.0, std::memory_order_relaxed);
    g_phase_global_y.store(0.0, std::memory_order_relaxed);
    g_phase_global_z.store(0.0, std::memory_order_relaxed);
    g_pauliz_global_x.store(0.0, std::memory_order_relaxed);
    g_pauliz_global_y.store(0.0, std::memory_order_relaxed);
    g_pauliz_global_z.store(0.0, std::memory_order_relaxed);
    g_cnot_pop_00.store(0.0, std::memory_order_relaxed);
    g_cnot_pop_01.store(0.0, std::memory_order_relaxed);
    g_cnot_pop_10.store(0.0, std::memory_order_relaxed);
    g_cnot_pop_11.store(0.0, std::memory_order_relaxed);
    
    g_cnot_coh_03_real.store(0.0, std::memory_order_relaxed);
    g_cnot_coh_03_imag.store(0.0, std::memory_order_relaxed);

    uint64_t rna_checksum_accumulator = 0;
    uint64_t quantum_sq_accumulator = 0;
    
    constexpr int64_t SCALE = 676LL;

    int64_t local_x_sum_s = 0, local_y_sum_s = 0, local_z_sum_s = 0;
    int64_t local_x_hadamard_sum_s = 0, local_y_hadamard_sum_s = 0, local_z_hadamard_sum_s = 0;
    int64_t local_x_phase_sum_s = 0, local_y_phase_sum_s = 0, local_z_phase_sum_s = 0;
    int64_t local_x_pauliz_sum_s = 0, local_y_pauliz_sum_s = 0, local_z_pauliz_sum_s = 0;
    int64_t local_cnot_00_sum_s = 0, local_cnot_01_sum_s = 0, local_cnot_10_sum_s = 0, local_cnot_11_sum_s = 0;
    int64_t local_cnot_03_real_sum_s = 0;
    int64_t local_cnot_03_imag_sum_s = 0;

    omp_set_num_threads(4); 

    #pragma omp parallel reduction(+:rna_checksum_accumulator) reduction(+:quantum_sq_accumulator) \
                         reduction(+:local_x_sum_s) reduction(+:local_y_sum_s) reduction(+:local_z_sum_s) \
                         reduction(+:local_x_hadamard_sum_s) reduction(+:local_y_hadamard_sum_s) reduction(+:local_z_hadamard_sum_s) \
                         reduction(+:local_x_phase_sum_s) reduction(+:local_y_phase_sum_s) reduction(+:local_z_phase_sum_s) \
                         reduction(+:local_x_pauliz_sum_s) reduction(+:local_y_pauliz_sum_s) reduction(+:local_z_pauliz_sum_s) \
                         reduction(+:local_cnot_00_sum_s) reduction(+:local_cnot_01_sum_s) reduction(+:local_cnot_10_sum_s) reduction(+:local_cnot_11_sum_s) \
                         reduction(+:local_cnot_03_real_sum_s) reduction(+:local_cnot_03_imag_sum_s)
    {
        uint64_t local_last_b = BAZA_A0;

        #pragma omp for schedule(guided, 1048576)
        for (int64_t i = 0; i < total_loops; ++i) {
            uint64_t val = BAZA_A0 + GTS_KROK * static_cast<uint64_t>(i);
            
            uint64_t dynamic_packet_seed = initial_seed ^ (static_cast<uint64_t>(i) * 3141592653589793238ULL);
            
            uint64_t dynamic_state = execute_p3_takt_pure(static_cast<uint64_t>(i), dynamic_packet_seed);
            rna_checksum_accumulator += dynamic_state;

            uint64_t local_sq = execute_quantum_sq_operator(dynamic_state);
            quantum_sq_accumulator += local_sq;

            uint64_t faza_u = dynamic_state % HORYZONT_1170;
            uint64_t faza_v = dynamic_state % PHI_44_SCALE;

            double raw_x = (static_cast<double>(faza_u) / 1170.0) * 2.0 - 1.0;
            double raw_y = (static_cast<double>(faza_v) / static_cast<double>(PHI_44_SCALE)) * 2.0 - 1.0;

            double r2 = raw_x * raw_x + raw_y * raw_y;
            double inv_denom = 1.0 / (1.0 + r2);

            double current_x = 2.0 * raw_x * inv_denom;
            double current_y = 2.0 * raw_y * inv_denom;
            double current_z = (1.0 - r2) * inv_denom;

            local_x_sum_s += static_cast<int64_t>(current_x * SCALE);
            local_y_sum_s += static_cast<int64_t>(current_y * SCALE);
            local_z_sum_s += static_cast<int64_t>(current_z * SCALE);

            double inv_sqrt2 = 0.70710678118654752440;
            double cur_hadamard_x = (current_x + current_z) * inv_sqrt2;
            double cur_hadamard_y = -current_y;
            double cur_hadamard_z = (current_x - current_z) * inv_sqrt2;
            local_x_hadamard_sum_s += static_cast<int64_t>(cur_hadamard_x * SCALE);
            local_y_hadamard_sum_s += static_cast<int64_t>(cur_hadamard_y * SCALE);
            local_z_hadamard_sum_s += static_cast<int64_t>(cur_hadamard_z * SCALE);

            double cur_phase_x = -cur_hadamard_y;
            double cur_phase_y = cur_hadamard_x;
            double cur_phase_z = cur_hadamard_z;
            local_x_phase_sum_s += static_cast<int64_t>(cur_phase_x * SCALE);
            local_y_phase_sum_s += static_cast<int64_t>(cur_phase_y * SCALE);
            local_z_phase_sum_s += static_cast<int64_t>(cur_phase_z * SCALE);

            local_x_pauliz_sum_s += static_cast<int64_t>(-cur_phase_x * SCALE);
            local_y_pauliz_sum_s += static_cast<int64_t>(-cur_phase_y * SCALE);
            local_z_pauliz_sum_s += static_cast<int64_t>(cur_phase_z * SCALE);

            double rho_c_00 = 0.5 * (1.0 + cur_hadamard_z);
            double rho_c_11 = 0.5 * (1.0 - cur_hadamard_z);
            
            double rho_c_01_real = 0.5 * cur_phase_x;
            double rho_c_10_real = 0.5 * cur_phase_y;
            
            local_cnot_00_sum_s += static_cast<int64_t>(rho_c_00 * SCALE);
            local_cnot_01_sum_s += static_cast<int64_t>(rho_c_01_real * SCALE);
            local_cnot_10_sum_s += static_cast<int64_t>(rho_c_10_real * SCALE);
            local_cnot_11_sum_s += static_cast<int64_t>(rho_c_11 * SCALE);

            double rho_c_03_real = 0.5 * cur_hadamard_x;
            double rho_c_03_imag = 0.5 * cur_hadamard_y;

            local_cnot_03_real_sum_s += static_cast<int64_t>(rho_c_03_real * SCALE);
            local_cnot_03_imag_sum_s += static_cast<int64_t>(rho_c_03_imag * SCALE);

            if ((i & 1048575) == 0) {
                int64_t current = g_global_quantum_counter.fetch_add(1048576, std::memory_order_relaxed) + 1048576;
                if (current >= CYKL_HOLO_GIGA && g_holographic_gear_tier.load(std::memory_order_relaxed) == 0) {
                    g_holographic_gear_tier.store(1, std::memory_order_relaxed);
                }
            }
        }
    }

    g_bloch_global_x.store(local_x_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_bloch_global_y.store(local_y_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_bloch_global_z.store(local_z_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_hadamard_global_x.store(local_x_hadamard_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_hadamard_global_y.store(local_y_hadamard_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_hadamard_global_z.store(local_z_hadamard_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_phase_global_x.store(local_x_phase_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_phase_global_y.store(local_y_phase_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_phase_global_z.store(local_z_phase_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_pauliz_global_x.store(local_x_pauliz_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_pauliz_global_y.store(local_y_pauliz_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_pauliz_global_z.store(local_z_pauliz_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_cnot_pop_00.store(local_cnot_00_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_cnot_pop_01.store(local_cnot_01_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_cnot_pop_10.store(local_cnot_10_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_cnot_pop_11.store(local_cnot_11_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    
    g_cnot_coh_03_real.store(local_cnot_03_real_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    g_cnot_coh_03_imag.store(local_cnot_03_imag_sum_s / static_cast<double>(SCALE), std::memory_order_relaxed);
    
    g_global_quantum_sq_sum.store(quantum_sq_accumulator, std::memory_order_relaxed);
    g_z2_safe_mode_popcount.store(__popcnt64(rna_checksum_accumulator), std::memory_order_relaxed);
    
    double p00_f = g_cnot_pop_00.load(std::memory_order_relaxed) / static_cast<double>(total_loops);
    double p11_f = g_cnot_pop_11.load(std::memory_order_relaxed) / static_cast<double>(total_loops);
    double c03_r = g_cnot_coh_03_real.load(std::memory_order_relaxed) / static_cast<double>(total_loops);
    double c03_i = g_cnot_coh_03_imag.load(std::memory_order_relaxed) / static_cast<double>(total_loops);
    double final_purity = (p00_f * p00_f) + (p11_f * p11_f) + 2.0 * ((c03_r * c03_r) + (c03_i * c03_i));

    std::cout << "  CZYSTOŚĆ KWANTOWA STANU: " << final_purity << "\n";

    return rna_checksum_accumulator;
}

py::dict get_quantum_telemetry(double total_loops) {
    py::dict d;
    double total_l = total_loops;

    double p00 = g_cnot_pop_00.load(std::memory_order_relaxed) / total_l;
    double p11 = g_cnot_pop_11.load(std::memory_order_relaxed) / total_l;
    double c03_r = g_cnot_coh_03_real.load(std::memory_order_relaxed) / total_l;
    double c03_i = g_cnot_coh_03_imag.load(std::memory_order_relaxed) / total_l;

    d["p00"] = p00;
    d["p11"] = p11;
    d["c03_real"] = c03_r;
    d["c03_imag"] = c03_i;

    d["trace"] = p00 + p11;
    d["bloch_z"] = g_bloch_global_z.load(std::memory_order_relaxed) / total_l;
    
    return d;
}

PYBIND11_MAKE_OPAQUE(std::vector<uint64_t>);

PYBIND11_MODULE(amsin_quantum_core, m) {
    py::bind_vector<std::vector<uint64_t>>(m, "VectorInt64");
    m.def("execute_quantum_simulation_zero_ram", &execute_quantum_simulation_zero_ram, 
          "Silnik Zero-RAM: dynamiczna generacja ziarna, brak alokacji, FPU-bound");
    m.def("get_quantum_telemetry", &get_quantum_telemetry, "Pasywny port obserwatora stanu czystego");
}
