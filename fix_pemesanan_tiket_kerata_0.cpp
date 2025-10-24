#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace std;

struct Stasiun {
    int id;
    string nama;
};

struct TipeKereta {
    int id;
    string nama;
    string kode;
};

struct JadwalKereta {
    string waktu;
    string estimasiTiba;
};

struct Tarif {
    string rute;
    int ekonomi;
    int bisnis;
    int eksekutif;
};

struct DataPenumpang {
    string nama;
    string noIdentitas;
    string stasiunAsal;
    string stasiunTujuan;
    string tipeKereta;
    string waktuBerangkat;
    string estimasiTiba;
    string kursi;
    int hargaTiket;
};

class SistemReservasiKereta {
private:
    vector<Stasiun> daftarStasiun;
    vector<TipeKereta> daftarTipe;
    vector<JadwalKereta> jadwalHarian;
    vector<Tarif> daftarTarif;
    DataPenumpang penumpang;

public:
    SistemReservasiKereta() {
        initData();
    }

    void initData() {
        // Inisialisasi stasiun
        daftarStasiun = {
            {1, "Jakarta"},
            {2, "Semarang"},
            {3, "Bandung"},
            {4, "Yogyakarta"},
            {5, "Surabaya"},
            {6, "Serang"}
        };

        // Inisialisasi tipe kereta
        daftarTipe = {
            {1, "Ekonomi", "EK"},
            {2, "Bisnis", "BS"},
            {3, "Eksekutif", "EKS"}
        };

        // Inisialisasi jadwal kereta (contoh jadwal harian)
        jadwalHarian = {
            {"06:00", "10:30"},
            {"08:00", "12:30"},
            {"10:00", "14:30"},
            {"12:00", "16:30"},
            {"14:00", "18:30"},
            {"16:00", "20:30"},
            {"18:00", "22:30"},
            {"20:00", "00:30"}
        };

        // Inisialisasi tarif berdasarkan rute dan tipe kereta
        daftarTarif = {
            // Rute dari Jakarta
            {"Jakarta-Semarang", 75000, 120000, 200000},
            {"Jakarta-Bandung", 50000, 85000, 150000},
            {"Jakarta-Yogyakarta", 90000, 150000, 250000},
            {"Jakarta-Surabaya", 120000, 180000, 300000},
            {"Jakarta-Serang", 35000, 60000, 100000},
            
            // Rute dari Semarang
            {"Semarang-Jakarta", 75000, 120000, 200000},
            {"Semarang-Bandung", 85000, 140000, 220000},
            {"Semarang-Yogyakarta", 45000, 75000, 125000},
            {"Semarang-Surabaya", 65000, 105000, 175000},
            {"Semarang-Serang", 95000, 155000, 260000},
            
            // Rute dari Bandung
            {"Bandung-Jakarta", 50000, 85000, 150000},
            {"Bandung-Semarang", 85000, 140000, 220000},
            {"Bandung-Yogyakarta", 70000, 115000, 185000},
            {"Bandung-Surabaya", 110000, 170000, 280000},
            {"Bandung-Serang", 40000, 70000, 120000},
            
            // Rute dari Yogyakarta
            {"Yogyakarta-Jakarta", 90000, 150000, 250000},
            {"Yogyakarta-Semarang", 45000, 75000, 125000},
            {"Yogyakarta-Bandung", 70000, 115000, 185000},
            {"Yogyakarta-Surabaya", 55000, 90000, 150000},
            {"Yogyakarta-Serang", 105000, 165000, 275000},
            
            // Rute dari Surabaya
            {"Surabaya-Jakarta", 120000, 180000, 300000},
            {"Surabaya-Semarang", 65000, 105000, 175000},
            {"Surabaya-Bandung", 110000, 170000, 280000},
            {"Surabaya-Yogyakarta", 55000, 90000, 150000},
            {"Surabaya-Serang", 140000, 210000, 350000},
            
            // Rute dari Serang
            {"Serang-Jakarta", 35000, 60000, 100000},
            {"Serang-Semarang", 95000, 155000, 260000},
            {"Serang-Bandung", 40000, 70000, 120000},
            {"Serang-Yogyakarta", 105000, 165000, 275000},
            {"Serang-Surabaya", 140000, 210000, 350000}
        };
    }

    void tampilkanHeader() {
        cout << "\n========================================\n";
        cout << "    SISTEM RESERVASI TIKET KERETA API\n";
        cout << "========================================\n";
    }

    void pilihStasiun() {
        int asalChoice, tujuanChoice;
        
        cout << "\n=== PILIH STASIUN KEBERANGKATAN ===\n";
        for (const auto& stasiun : daftarStasiun) {
            cout << stasiun.id << ". " << stasiun.nama << endl;
        }
        cout << "Pilih stasiun asal (1-6): ";
        cin >> asalChoice;

        cout << "\n=== PILIH STASIUN TUJUAN ===\n";
        for (const auto& stasiun : daftarStasiun) {
            if (stasiun.id != asalChoice) {
                cout << stasiun.id << ". " << stasiun.nama << endl;
            }
        }
        cout << "Pilih stasiun tujuan: ";
        cin >> tujuanChoice;

        penumpang.stasiunAsal = daftarStasiun[asalChoice-1].nama;
        penumpang.stasiunTujuan = daftarStasiun[tujuanChoice-1].nama;
    }

    void pilihTipeKereta() {
        int tipeChoice;
        
        cout << "\n=== PILIH TIPE KERETA ===\n";
        for (const auto& tipe : daftarTipe) {
            cout << tipe.id << ". " << tipe.nama << endl;
        }
        cout << "Pilih tipe kereta (1-3): ";
        cin >> tipeChoice;

        penumpang.tipeKereta = daftarTipe[tipeChoice-1].nama;
        
        // Hitung tarif
        string rute = penumpang.stasiunAsal + "-" + penumpang.stasiunTujuan;
        for (const auto& tarif : daftarTarif) {
            if (tarif.rute == rute) {
                if (tipeChoice == 1) penumpang.hargaTiket = tarif.ekonomi;
                else if (tipeChoice == 2) penumpang.hargaTiket = tarif.bisnis;
                else penumpang.hargaTiket = tarif.eksekutif;
                break;
            }
        }
    }

    void pilihWaktuKeberangkatan() {
        int waktuChoice;
        
        cout << "\n=== PILIH WAKTU KEBERANGKATAN ===\n";
        for (size_t i = 0; i < jadwalHarian.size(); i++) {
            cout << (i+1) << ". " << jadwalHarian[i].waktu 
                 << " (Estimasi tiba: " << jadwalHarian[i].estimasiTiba << ")" << endl;
        }
        cout << "Pilih waktu keberangkatan (1-" << jadwalHarian.size() << "): ";
        cin >> waktuChoice;

        penumpang.waktuBerangkat = jadwalHarian[waktuChoice-1].waktu;
        penumpang.estimasiTiba = jadwalHarian[waktuChoice-1].estimasiTiba;
    }

    void isiDataDiri() {
        cin.ignore(); // Clear buffer
        cout << "\n=== ISI DATA DIRI ===\n";
        cout << "Nama Lengkap: ";
        getline(cin, penumpang.nama);
        cout << "Nomor Identitas (KTP/SIM): ";
        getline(cin, penumpang.noIdentitas);
    }

    void pilihKursi() {
        string kodeKereta;
        for (const auto& tipe : daftarTipe) {
            if (tipe.nama == penumpang.tipeKereta) {
                kodeKereta = tipe.kode;
                break;
            }
        }

        cout << "\n=== PILIH KURSI ===\n";
        cout << "Tipe kereta: " << penumpang.tipeKereta << " (" << kodeKereta << ")\n";
        cout << "Kursi yang tersedia: \n";
        
        // Simulasi kursi tersedia
        vector<string> kursiTersedia = {"12A", "12B", "13A", "13B", "14A", "14B", "15A", "15B"};
        
        for (size_t i = 0; i < kursiTersedia.size(); i++) {
            cout << (i+1) << ". " << kodeKereta << " 1 : " << kursiTersedia[i] << endl;
        }
        
        int kursiChoice;
        cout << "Pilih kursi (1-" << kursiTersedia.size() << "): ";
        cin >> kursiChoice;
        
        penumpang.kursi = kodeKereta + " 1 : " + kursiTersedia[kursiChoice-1];
    }

    int pilihPembayaran() {
        int metodePembayaran;
        
        cout << "\n=== METODE PEMBAYARAN ===\n";
        cout << "Total yang harus dibayar: Rp " << penumpang.hargaTiket << endl;
        cout << "1. Bank Transfer\n";
        cout << "2. QRIS\n";
        cout << "Pilih metode pembayaran (1-2): ";
        cin >> metodePembayaran;
        
        return metodePembayaran;
    }

    void tampilkanTiket() {
        cout << "\n" << string(50, '=') << endl;
        cout << "              TIKET KERETA API" << endl;
        cout << string(50, '=') << endl;
        
        // Get current time for ticket number
        time_t now = time(0);
        struct tm* timeinfo = localtime(&now);
        
        cout << "No. Tiket    : TK" << (timeinfo->tm_year + 1900) 
             << setfill('0') << setw(2) << (timeinfo->tm_mon + 1)
             << setw(2) << timeinfo->tm_mday 
             << setw(2) << timeinfo->tm_hour
             << setw(2) << timeinfo->tm_min << endl;
        
        cout << string(50, '-') << endl;
        cout << "Nama         : " << penumpang.nama << endl;
        cout << "No. Identitas: " << penumpang.noIdentitas << endl;
        cout << string(50, '-') << endl;
        cout << "Stasiun Asal : " << penumpang.stasiunAsal << endl;
        cout << "Stasiun Tujuan: " << penumpang.stasiunTujuan << endl;
        cout << "Tipe Kereta  : " << penumpang.tipeKereta << endl;
        cout << "Kursi        : " << penumpang.kursi << endl;
        cout << string(50, '-') << endl;
        cout << "Waktu Berangkat: " << penumpang.waktuBerangkat << endl;
        cout << "Estimasi Tiba  : " << penumpang.estimasiTiba << endl;
        cout << string(50, '-') << endl;
        cout << "Harga Tiket  : Rp " << penumpang.hargaTiket << endl;
        cout << string(50, '=') << endl;
        cout << "     Terima kasih telah memilih layanan kami!" << endl;
        cout << "      Harap tiba di stasiun 30 menit sebelum" << endl;
        cout << "              waktu keberangkatan" << endl;
        cout << string(50, '=') << endl;
    }

    void jalankanSistem() {
        while (true) {
            tampilkanHeader();
            
            // Langkah 1: Pilih Stasiun
            pilihStasiun();
            
            // Langkah 2: Pilih Tipe Kereta
            pilihTipeKereta();
            
            bool lanjutkan = true;
            while (lanjutkan) {
                // Langkah 3: Pilih Waktu Keberangkatan
                pilihWaktuKeberangkatan();
                
                // Langkah 4: Tarif otomatis dihitung
                
                // Langkah 5: Isi Data Diri
                isiDataDiri();
                
                // Langkah 6: Pilih Kursi
                pilihKursi();
                
                // Langkah 7: Pembayaran
                int metodePembayaran = pilihPembayaran();
                
                // Langkah 8: Konfirmasi
                cout << "\n=== KONFIRMASI PESANAN ===\n";
                cout << "1. Lanjutkan dan cetak tiket\n";
                cout << "2. Ubah jadwal keberangkatan\n";
                cout << "3. Batalkan pesanan\n";
                cout << "Pilihan Anda: ";
                
                int konfirmasi;
                cin >> konfirmasi;
                
                if (konfirmasi == 1) {
                    // Simulasi proses pembayaran
                    cout << "\nMemproses pembayaran";
                    for (int i = 0; i < 3; i++) {
                        cout << ".";
                        cout.flush();
                        // Simulasi delay
                        for (int j = 0; j < 100000000; j++);
                    }
                    cout << " Berhasil!\n";
                    
                    // Cetak tiket
                    tampilkanTiket();
                    lanjutkan = false;
                } else if (konfirmasi == 2) {
                    // Kembali ke pilih waktu keberangkatan
                    continue;
                } else {
                    cout << "\nPesanan dibatalkan. Terima kasih!\n";
                    lanjutkan = false;
                }
            }
            
            // Tanya apakah ingin membuat pesanan lagi
            cout << "\nApakah Anda ingin membuat pesanan lagi? (y/n): ";
            char ulang;
            cin >> ulang;
            if (ulang != 'y' && ulang != 'Y') {
                break;
            }
        }
        
        cout << "\nTerima kasih telah menggunakan sistem reservasi kami!\n";
    }
};

int main() {
    SistemReservasiKereta sistem;
    sistem.jalankanSistem();
    return 0;
}