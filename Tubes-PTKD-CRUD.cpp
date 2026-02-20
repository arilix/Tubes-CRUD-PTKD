#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <ctime>
#include <vector>

using namespace std;

// ===============================================================
// KONSTANTA GLOBAL
// ===============================================================
const int MAX_DATA = 100;
const int MAX_USERS = 20;
const int MAX_STASIUN = 50;
const int MAX_KERETA = 20;

// ===============================================================
// STRUKTUR DATA
// ===============================================================
struct User {
    int id; // ID unik untuk setiap user
    string username;
    string password;
    string role; // "admin" atau "user"
    bool isActive;
};

struct Stasiun {
    int id;
    string nama;
    string kota;
    bool isActive;
};

struct Kereta {
    int id;
    string nama;
    string tipe;
    int kapasitas;
    int hargaPerKm;
    bool isActive;
};

struct Pemesanan {
    int id;
    int id_pengguna; // KUNCI: ID pengguna yang membuat pesanan ini
    string namaPenumpang;
    string noKTP;
    string noTelp;
    int idStasiunAsal;
    int idStasiunTujuan;
    int idKereta;
    string tanggalBerangkat;
    int jumlahTiket;
    int totalHarga;
    string status;
    string tanggalPesan;
    bool isActive;
};

// ===============================================================
// ARRAY GLOBAL & VARIABEL STATUS
// ===============================================================
User users[MAX_USERS];
Stasiun stasiuns[MAX_STASIUN];
Kereta keretas[MAX_KERETA];
Pemesanan pemesanans[MAX_DATA];
int totalUsers = 0;
int totalStasiuns = 0;
int totalKeretas = 0;
int totalPemesanans = 0;
int currentUserIndex = -1; // Menyimpan INDEKS array dari user yang login

// ===============================================================
// DEKLARASI FUNGSI (FUNCTION PROTOTYPES)
// ===============================================================
void clearScreen();
void pauseScreen();
string getCurrentDate();
string formatRupiah(long long amount); // <-- PERBAIKAN DI SINI
bool validasiNama(const string& nama);
bool validasiKTP(const string& ktp);
bool validasiTelp(const string& telp);
void loadUsers();
void saveUsers();
void loadStasiuns();
void saveStasiuns();
void loadKeretas();
void saveKeretas();
void loadPemesanans();
void savePemesanans();
bool login();
int cariStasiunById(int id);
int cariKeretaById(int id);
int cariPemesananById(int id);
void tambahPemesanan();
void tampilkanPemesanan();
void updatePemesanan();
void hapusPemesanan();
void dashboard();
void cetakNota();
void tampilkanMenu();
void buatAkun();
int main();

// ===============================================================
// FUNGSI UTILITAS
// ===============================================================
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", ltm);
    return string(buffer);
}

string formatRupiah(long long amount) {
    string result = "Rp ";
    string num = to_string(amount);
    int len = num.length();
    for (int i = 0; i < len; i++) {
        if (i > 0 && (len - i) % 3 == 0) {
            result += ".";
        }
        result += num[i];
    }
    return result;
}

// ===============================================================
// FUNGSI VALIDASI INPUT
// ===============================================================
bool validasiNama(const string& nama) {
    if (nama.length() < 3 || nama.length() > 50) return false;
    for (char c : nama) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return true;
}

bool validasiKTP(const string& ktp) {
    if (ktp.length() != 16) return false;
    for (char c : ktp) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool validasiTelp(const string& telp) {
    if (telp.length() < 10 || telp.length() > 15) return false;
    for (char c : telp) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// ===============================================================
// FUNGSI FILE I/O
// ===============================================================
void saveUsers() {
    ofstream file("users.csv");
    if (!file.is_open()) {
        cerr << "Error: Tidak bisa membuka file users.csv untuk ditulis." << endl;
        return;
    }
    file << "ID,Username,Password,Role,IsActive" << endl;
    for (int i = 0; i < totalUsers; i++) {
        file << users[i].id << "," << users[i].username << "," << users[i].password << "," << users[i].role << "," << users[i].isActive << endl;
    }
    file.close();
}

void loadUsers() {
    ifstream file("users.csv");
    totalUsers = 0;
    string line, cell;
    if (file.is_open()) {
        getline(file, line); // Skip header
        while (getline(file, line) && totalUsers < MAX_USERS) {
            stringstream ss(line);
            vector<string> data;
            while(getline(ss, cell, ',')) {
                data.push_back(cell);
            }
            if(data.size() == 5) {
                users[totalUsers].id = stoi(data[0]);
                users[totalUsers].username = data[1];
                users[totalUsers].password = data[2];
                users[totalUsers].role = data[3];
                users[totalUsers].isActive = (data[4] == "1");
                totalUsers++;
            }
        }
        file.close();
    }
    if (totalUsers == 0) {
        users[0] = {1, "admin", "admin123", "admin", true};
        users[1] = {2, "user1", "pass123", "user", true};
        totalUsers = 2;
        saveUsers();
    }
}

void saveStasiuns() {
    ofstream file("stasiuns.csv");
    if (!file.is_open()) {
        cerr << "Error: Tidak bisa membuka file stasiuns.csv untuk ditulis." << endl;
        return;
    }
    file << "ID,Nama,Kota,IsActive" << endl;
    for (int i = 0; i < totalStasiuns; i++) {
        file << stasiuns[i].id << "," << stasiuns[i].nama << "," << stasiuns[i].kota << "," << stasiuns[i].isActive << endl;
    }
    file.close();
}

void loadStasiuns() {
    ifstream file("stasiuns.csv");
    string line, cell;
    totalStasiuns = 0;
    if (file.is_open()) {
        getline(file, line);
        while (getline(file, line) && totalStasiuns < MAX_STASIUN) {
            stringstream ss(line);
            vector<string> data;
            while(getline(ss, cell, ',')) data.push_back(cell);
            if (data.size() == 4) {
                stasiuns[totalStasiuns] = {stoi(data[0]), data[1], data[2], (data[3] == "1")};
                totalStasiuns++;
            }
        }
        file.close();
    }
    if (totalStasiuns == 0) {
        stasiuns[0] = {1, "Stasiun Gambir", "Jakarta", true};
        stasiuns[1] = {2, "Stasiun Bandung", "Bandung", true};
        stasiuns[2] = {3, "Stasiun Yogyakarta", "Yogyakarta", true};
        stasiuns[3] = {4, "Stasiun Solo Balapan", "Solo", true};
        stasiuns[4] = {5, "Stasiun Surabaya Gubeng", "Surabaya", true};
        totalStasiuns = 5;
        saveStasiuns();
    }
}

void saveKeretas() {
    ofstream file("keretas.csv");
    if (!file.is_open()) {
        cerr << "Error: Tidak bisa membuka file keretas.csv untuk ditulis." << endl;
        return;
    }
    file << "ID,Nama,Tipe,Kapasitas,HargaPerKm,IsActive" << endl;
    for (int i = 0; i < totalKeretas; i++) {
        file << keretas[i].id << "," << keretas[i].nama << "," << keretas[i].tipe << ","
             << keretas[i].kapasitas << "," << keretas[i].hargaPerKm << "," << keretas[i].isActive << endl;
    }
    file.close();
}

void loadKeretas() {
    ifstream file("keretas.csv");
    string line, cell;
    totalKeretas = 0;
    if (file.is_open()) {
        getline(file, line);
        while (getline(file, line) && totalKeretas < MAX_KERETA) {
            stringstream ss(line);
            vector<string> data;
            while(getline(ss, cell, ',')) data.push_back(cell);
            if (data.size() == 6) {
                keretas[totalKeretas] = {stoi(data[0]), data[1], data[2], stoi(data[3]), stoi(data[4]), (data[5] == "1")};
                totalKeretas++;
            }
        }
        file.close();
    }
    if (totalKeretas == 0) {
        keretas[0] = {1, "Argo Bromo Anggrek", "Eksekutif", 200, 500, true};
        keretas[1] = {2, "Bima", "Bisnis", 300, 300, true};
        keretas[2] = {3, "Gaya Baru Malam", "Ekonomi", 400, 200, true};
        keretas[3] = {4, "Argo Lawu", "Eksekutif", 200, 450, true};
        totalKeretas = 4;
        saveKeretas();
    }
}

void savePemesanans() {
    ofstream file("pemesanans.csv");
    if (!file.is_open()) {
        cerr << "Error: Tidak bisa membuka file pemesanans.csv untuk ditulis." << endl;
        return;
    }
    file << "ID,IDPengguna,Nama,KTP,Telp,StasiunAsal,StasiunTujuan,Kereta,TglBerangkat,JumlahTiket,TotalHarga,Status,TglPesan,IsActive" << endl;
    for (int i = 0; i < totalPemesanans; i++) {
        file << pemesanans[i].id << "," << pemesanans[i].id_pengguna << "," << pemesanans[i].namaPenumpang << ","
             << pemesanans[i].noKTP << "," << pemesanans[i].noTelp << ","
             << pemesanans[i].idStasiunAsal << "," << pemesanans[i].idStasiunTujuan << ","
             << pemesanans[i].idKereta << "," << pemesanans[i].tanggalBerangkat << ","
             << pemesanans[i].jumlahTiket << "," << pemesanans[i].totalHarga << ","
             << pemesanans[i].status << "," << pemesanans[i].tanggalPesan << ","
             << pemesanans[i].isActive << endl;
    }
    file.close();
}

void loadPemesanans() {
    ifstream file("pemesanans.csv");
    string line, cell;
    totalPemesanans = 0;
    if (file.is_open()) {
        getline(file, line);
        while (getline(file, line) && totalPemesanans < MAX_DATA) {
            stringstream ss(line);
            vector<string> data;
            while(getline(ss, cell, ',')) data.push_back(cell);
            if (data.size() == 14) {
                pemesanans[totalPemesanans].id = stoi(data[0]);
                pemesanans[totalPemesanans].id_pengguna = stoi(data[1]);
                pemesanans[totalPemesanans].namaPenumpang = data[2];
                pemesanans[totalPemesanans].noKTP = data[3];
                pemesanans[totalPemesanans].noTelp = data[4];
                pemesanans[totalPemesanans].idStasiunAsal = stoi(data[5]);
                pemesanans[totalPemesanans].idStasiunTujuan = stoi(data[6]);
                pemesanans[totalPemesanans].idKereta = stoi(data[7]);
                pemesanans[totalPemesanans].tanggalBerangkat = data[8];
                pemesanans[totalPemesanans].jumlahTiket = stoi(data[9]);
                pemesanans[totalPemesanans].totalHarga = stoi(data[10]);
                pemesanans[totalPemesanans].status = data[11];
                pemesanans[totalPemesanans].tanggalPesan = data[12];
                pemesanans[totalPemesanans].isActive = (data[13] == "1");
                totalPemesanans++;
            }
        }
        file.close();
    }
}

// ===============================================================
// FUNGSI AUTENTIKASI
// ===============================================================
bool login() {
    string username, password;
    int attempts = 0;
    while (attempts < 3) {
        clearScreen();
        cout << "===============================================" << endl;
        cout << "         SISTEM LOGIN TIKET KERETA" << endl;
        cout << "===============================================" << endl;
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        for (int i = 0; i < totalUsers; i++) {
            if (users[i].username == username && users[i].password == password && users[i].isActive) {
                currentUserIndex = i; // Simpan INDEKS user yang login
                cout << "\nLogin berhasil! Selamat datang, " << username << endl;
                pauseScreen();
                return true;
            }
        }
        attempts++;
        cout << "\nUsername atau password salah! Sisa percobaan: " << (3 - attempts) << endl;
        if (attempts < 3) pauseScreen();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "\nTerlalu banyak percobaan gagal. Program akan keluar." << endl;
    return false;
}

// ===============================================================
// FUNGSI BUAT AKUN BARU
// ===============================================================
void buatAkun() {
    clearScreen();
    cout << "===============================================" << endl;
    cout << "             BUAT AKUN BARU" << endl;
    cout << "===============================================" << endl;

    if (totalUsers >= MAX_USERS) {
        cout << "Maaf, kapasitas pengguna sudah penuh!" << endl;
        pauseScreen();
        return;
    }

    User newUser;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    cout << "Username: ";
    getline(cin, newUser.username);
    while (newUser.username.empty() || newUser.username.length() > 20) {
        cout << "Username tidak valid! Masukkan 1-20 karakter: ";
        getline(cin, newUser.username);
    }

    for (int i = 0; i < totalUsers; i++) {
        if (users[i].username == newUser.username) {
            cout << "Username sudah digunakan! Silakan coba yang lain." << endl;
            pauseScreen();
            return;
        }
    }

    cout << "Password: ";
    getline(cin, newUser.password);
    while (newUser.password.empty() || newUser.password.length() < 6) {
        cout << "Password tidak valid! Minimal 6 karakter: ";
        getline(cin, newUser.password);
    }
    
    // Memberikan ID unik dan role default
    newUser.id = (totalUsers == 0) ? 1 : users[totalUsers - 1].id + 1;
    newUser.role = "user"; // Semua akun baru otomatis jadi user biasa
    newUser.isActive = true;

    users[totalUsers] = newUser;
    totalUsers++;
    saveUsers();

    cout << "\nAkun berhasil dibuat dengan ID: " << newUser.id << "!" << endl;
    pauseScreen();
}

// ===============================================================
// FUNGSI PENCARIAN
// ===============================================================
int cariStasiunById(int id) {
    for (int i = 0; i < totalStasiuns; i++) {
        if (stasiuns[i].id == id && stasiuns[i].isActive) return i;
    }
    return -1;
}

int cariKeretaById(int id) {
    for (int i = 0; i < totalKeretas; i++) {
        if (keretas[i].id == id && keretas[i].isActive) return i;
    }
    return -1;
}

int cariPemesananById(int id) {
    for (int i = 0; i < totalPemesanans; i++) {
        if (pemesanans[i].id == id && pemesanans[i].isActive) return i;
    }
    return -1;
}

// ===============================================================
// FUNGSI CRUD - CREATE
// ===============================================================
void tambahPemesanan() {
    clearScreen();
    cout << "===============================================" << endl;
    cout << "             TAMBAH PEMESANAN TIKET" << endl;
    cout << "===============================================" << endl;
    if (totalPemesanans >= MAX_DATA) {
        cout << "Maaf, kapasitas pemesanan sudah penuh!" << endl;
        pauseScreen();
        return;
    }
    Pemesanan baru;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Nama Penumpang: ";
    getline(cin, baru.namaPenumpang);
    while (!validasiNama(baru.namaPenumpang)) {
        cout << "Nama tidak valid! Masukkan nama (3-50 karakter, huruf saja): ";
        getline(cin, baru.namaPenumpang);
    }
    cout << "No KTP: ";
    cin >> baru.noKTP;
    while (!validasiKTP(baru.noKTP)) {
        cout << "No KTP tidak valid! Masukkan 16 digit angka: ";
        cin >> baru.noKTP;
    }
    cout << "No Telepon: ";
    cin >> baru.noTelp;
    while (!validasiTelp(baru.noTelp)) {
        cout << "No telepon tidak valid! Masukkan 10-15 digit: ";
        cin >> baru.noTelp;
    }

    cout << "\nDaftar Stasiun:" << endl;
    cout << left << setw(5) << "ID" << setw(25) << "Nama Stasiun" << "Kota" << endl;
    cout << string(45, '-') << endl;
    for (int i = 0; i < totalStasiuns; i++) {
        if (stasiuns[i].isActive) {
            cout << left << setw(5) << stasiuns[i].id << setw(25) << stasiuns[i].nama << stasiuns[i].kota << endl;
        }
    }

    cout << "\nPilih Stasiun Asal (ID): ";
    cin >> baru.idStasiunAsal;
    while (cariStasiunById(baru.idStasiunAsal) == -1) {
        cout << "Stasiun tidak ditemukan! Pilih ID yang valid: ";
        cin >> baru.idStasiunAsal;
    }
    cout << "Pilih Stasiun Tujuan (ID): ";
    cin >> baru.idStasiunTujuan;
    while (cariStasiunById(baru.idStasiunTujuan) == -1 || baru.idStasiunTujuan == baru.idStasiunAsal) {
        cout << "Stasiun tidak valid! Pilih ID yang berbeda dari stasiun asal: ";
        cin >> baru.idStasiunTujuan;
    }

    cout << "\nDaftar Kereta:" << endl;
    cout << left << setw(5) << "ID" << setw(25) << "Nama Kereta" << setw(15) << "Tipe" << "Harga/km" << endl;
    cout << string(65, '-') << endl;
    for (int i = 0; i < totalKeretas; i++) {
        if (keretas[i].isActive) {
            cout << left << setw(5) << keretas[i].id << setw(25) << keretas[i].nama << setw(15) << keretas[i].tipe << formatRupiah(keretas[i].hargaPerKm) << endl;
        }
    }

    cout << "\nPilih Kereta (ID): ";
    cin >> baru.idKereta;
    while (cariKeretaById(baru.idKereta) == -1) {
        cout << "Kereta tidak ditemukan! Pilih ID yang valid: ";
        cin >> baru.idKereta;
    }
    cout << "Tanggal Berangkat (DD/MM/YYYY): ";
    cin >> baru.tanggalBerangkat;
    cout << "Jumlah Tiket: ";
    cin >> baru.jumlahTiket;
    while (cin.fail() || baru.jumlahTiket <= 0 || baru.jumlahTiket > 10) {
        cout << "Jumlah tiket tidak valid! Masukkan 1-10: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> baru.jumlahTiket;
    }

    int indexKereta = cariKeretaById(baru.idKereta);
    int jarak = 100 * abs(baru.idStasiunTujuan - baru.idStasiunAsal);
    baru.totalHarga = keretas[indexKereta].hargaPerKm * jarak * baru.jumlahTiket;

    baru.id = (totalPemesanans == 0) ? 1 : pemesanans[totalPemesanans - 1].id + 1;
    baru.id_pengguna = users[currentUserIndex].id; // Menghubungkan pesanan dengan user yang login
    baru.status = "Booked";
    baru.tanggalPesan = getCurrentDate();
    baru.isActive = true;

    pemesanans[totalPemesanans] = baru;
    totalPemesanans++;
    cout << "\n===============================================" << endl;
    cout << "             PEMESANAN BERHASIL!" << endl;
    cout << "===============================================" << endl;
    cout << "ID Pemesanan: " << baru.id << endl;
    cout << "Total Bayar : " << formatRupiah(baru.totalHarga) << endl;
    cout << "===============================================" << endl;
    savePemesanans();
    pauseScreen();
}

// ===============================================================
// FUNGSI CRUD - READ
// ===============================================================
void tampilkanPemesanan() {
    clearScreen();
    cout << "=====================================================================================================" << endl;
    cout << "                                        DAFTAR PEMESANAN TIKET" << endl;
    cout << "=====================================================================================================" << endl;
    
    User penggunaLogin = users[currentUserIndex];
    cout << "Menampilkan tiket untuk: " << penggunaLogin.username 
         << " (Role: " << penggunaLogin.role << ")" << endl << endl;

    int activeCount = 0;
    cout << left << setw(5) << "ID" << setw(20) << "Nama Penumpang" << setw(15) << "Asal" 
         << setw(15) << "Tujuan" << setw(20) << "Kereta" << setw(15) << "Total" << setw(12) << "Status";
    if (penggunaLogin.role == "admin") {
        cout << "Pemilik";
    }
    cout << endl;
    cout << string(115, '-') << endl;

    for (int i = 0; i < totalPemesanans; i++) {
        // KONDISI FILTER:
        // 1. Pesanan harus aktif (isActive)
        // 2. Jika pengguna adalah 'admin', tampilkan semua.
        // 3. Jika bukan 'admin', tampilkan hanya jika id_pengguna cocok.
        if (pemesanans[i].isActive && (penggunaLogin.role == "admin" || pemesanans[i].id_pengguna == penggunaLogin.id)) {
            activeCount++;
            int indexAsal = cariStasiunById(pemesanans[i].idStasiunAsal);
            int indexTujuan = cariStasiunById(pemesanans[i].idStasiunTujuan);
            int indexKereta = cariKeretaById(pemesanans[i].idKereta);
            
            cout << left << setw(5) << pemesanans[i].id 
                 << setw(20) << pemesanans[i].namaPenumpang.substr(0, 18)
                 << setw(15) << (indexAsal != -1 ? stasiuns[indexAsal].kota : "N/A")
                 << setw(15) << (indexTujuan != -1 ? stasiuns[indexTujuan].kota : "N/A")
                 << setw(20) << (indexKereta != -1 ? keretas[indexKereta].nama.substr(0,18) : "N/A")
                 << setw(15) << formatRupiah(pemesanans[i].totalHarga)
                 << setw(12) << pemesanans[i].status;

            if (penggunaLogin.role == "admin") {
                string ownerName = "N/A";
                for(int j = 0; j < totalUsers; ++j) {
                    if(users[j].id == pemesanans[i].id_pengguna) {
                        ownerName = users[j].username;
                        break;
                    }
                }
                cout << ownerName;
            }
            cout << endl;
        }
    }

    if (activeCount == 0) {
        cout << "\nTidak ada data pemesanan yang bisa ditampilkan." << endl;
    }
    pauseScreen();
}

// ===============================================================
// FUNGSI CRUD - UPDATE
// ===============================================================
void updatePemesanan() {
    clearScreen();
    cout << "===============================================" << endl;
    cout << "             UPDATE PEMESANAN TIKET" << endl;
    cout << "===============================================" << endl;
    int id;
    cout << "Masukkan ID Pemesanan yang akan diupdate: ";
    cin >> id;
    int index = cariPemesananById(id);
    if (index == -1) {
        cout << "Pemesanan dengan ID " << id << " tidak ditemukan!" << endl;
        pauseScreen();
        return;
    }

    // Cek Hak Akses
    User penggunaLogin = users[currentUserIndex];
    if (penggunaLogin.role != "admin" && pemesanans[index].id_pengguna != penggunaLogin.id) {
        cout << "\nError: Anda tidak memiliki izin untuk mengubah pesanan ini!" << endl;
        pauseScreen();
        return;
    }

    cout << "\nData saat ini:" << endl;
    cout << "Nama   : " << pemesanans[index].namaPenumpang << endl;
    cout << "Status : " << pemesanans[index].status << endl;
    cout << "\nPilihan update:" << endl;
    cout << "1. Update Status" << endl;
    cout << "2. Update Tanggal Berangkat" << endl;
    cout << "Pilih: ";
    int pilihan;
    cin >> pilihan;
    switch (pilihan) {
        case 1: {
            cout << "Status baru (Booked/Confirmed/Cancelled): ";
            cin >> pemesanans[index].status;
            break;
        }
        case 2: {
            cout << "Tanggal berangkat baru (DD/MM/YYYY): ";
            cin >> pemesanans[index].tanggalBerangkat;
            break;
        }
        default:
            cout << "Pilihan tidak valid!" << endl;
            pauseScreen();
            return;
    }
    cout << "\nData berhasil diupdate!" << endl;
    savePemesanans();
    pauseScreen();
}

// ===============================================================
// FUNGSI CRUD - DELETE
// ===============================================================
void hapusPemesanan() {
    clearScreen();
    cout << "===============================================" << endl;
    cout << "             HAPUS PEMESANAN TIKET" << endl;
    cout << "===============================================" << endl;
    int id;
    cout << "Masukkan ID Pemesanan yang akan dihapus: ";
    cin >> id;
    int index = cariPemesananById(id);
    if (index == -1) {
        cout << "Pemesanan dengan ID " << id << " tidak ditemukan!" << endl;
        pauseScreen();
        return;
    }

    // Cek Hak Akses
    User penggunaLogin = users[currentUserIndex];
    if (penggunaLogin.role != "admin" && pemesanans[index].id_pengguna != penggunaLogin.id) {
        cout << "\nError: Anda tidak memiliki izin untuk menghapus pesanan ini!" << endl;
        pauseScreen();
        return;
    }

    cout << "\nData yang akan dihapus:" << endl;
    cout << "ID   : " << pemesanans[index].id << endl;
    cout << "Nama : " << pemesanans[index].namaPenumpang << endl;
    cout << "Total: " << formatRupiah(pemesanans[index].totalHarga) << endl;
    cout << "\nYakin ingin menghapus? (y/n): ";
    char konfirmasi;
    cin >> konfirmasi;
    if (konfirmasi == 'y' || konfirmasi == 'Y') {
        pemesanans[index].isActive = false; // Soft delete
        cout << "\nPemesanan berhasil dihapus!" << endl;
        savePemesanans();
    } else {
        cout << "\nPenghapusan dibatalkan." << endl;
    }
    pauseScreen();
}

// ===============================================================
// FUNGSI DASHBOARD & STATISTIK
// ===============================================================
void dashboard() {
    clearScreen();
    cout << "===============================================" << endl;
    cout << "              DASHBOARD & STATISTIK" << endl;
    cout << "===============================================" << endl;
    int totalAktif = 0;
    long long totalPendapatan = 0;
    int totalTiket = 0;
    for (int i = 0; i < totalPemesanans; i++) {
        if (pemesanans[i].isActive && pemesanans[i].status != "Cancelled") {
            totalAktif++;
            totalPendapatan += pemesanans[i].totalHarga;
            totalTiket += pemesanans[i].jumlahTiket;
        }
    }
    cout << "Total Pemesanan Aktif   : " << totalAktif << endl;
    cout << "Total Tiket Terjual       : " << totalTiket << endl;
    cout << "Total Pendapatan          : " << formatRupiah(totalPendapatan) << endl;
    if (totalAktif > 0) {
        cout << "Rata-rata per Pemesanan : " << formatRupiah(totalPendapatan / totalAktif) << endl;
    }
    
    vector<int> keretaCount(totalKeretas, 0);
    for (int i = 0; i < totalPemesanans; i++) {
        if (pemesanans[i].isActive) {
            int keretaIdx = cariKeretaById(pemesanans[i].idKereta);
            if(keretaIdx != -1) {
                 keretaCount[keretaIdx]++;
            }
        }
    }
    int maxCount = 0;
    int popularKeretaIndex = -1;
    for (int i = 0; i < totalKeretas; i++) {
        if (keretaCount[i] > maxCount) {
            maxCount = keretaCount[i];
            popularKeretaIndex = i;
        }
    }
    cout << "-----------------------------------------------" << endl;
    if (popularKeretaIndex != -1) {
        cout << "Kereta Terpopuler         : " << keretas[popularKeretaIndex].nama << " (" << maxCount << " pemesanan)" << endl;
    } else {
        cout << "Kereta Terpopuler         : Belum ada data" << endl;
    }
    pauseScreen();
}

// ===============================================================
// FUNGSI CETAK NOTA
// ===============================================================
void cetakNota() {
    clearScreen();
    cout << "===============================================" << endl;
    cout << "                CETAK NOTA TIKET" << endl;
    cout << "===============================================" << endl;
    int id;
    cout << "Masukkan ID Pemesanan: ";
    cin >> id;
    int index = cariPemesananById(id);
    if (index == -1) {
        cout << "Pemesanan dengan ID " << id << " tidak ditemukan!" << endl;
        pauseScreen();
        return;
    }

    // Cek Hak Akses
    User penggunaLogin = users[currentUserIndex];
    if (penggunaLogin.role != "admin" && pemesanans[index].id_pengguna != penggunaLogin.id) {
        cout << "\nError: Anda tidak memiliki izin untuk mencetak nota ini!" << endl;
        pauseScreen();
        return;
    }

    Pemesanan p = pemesanans[index];
    int indexAsal = cariStasiunById(p.idStasiunAsal);
    int indexTujuan = cariStasiunById(p.idStasiunTujuan);
    int indexKereta = cariKeretaById(p.idKereta);

    stringstream notaContent;
    notaContent << "===============================================\n";
    notaContent << "               TIKET KERETA API\n";
    notaContent << "===============================================\n";
    notaContent << left << setw(20) << "ID Pemesanan" << ": " << p.id << endl;
    notaContent << left << setw(20) << "Nama Penumpang" << ": " << p.namaPenumpang << endl;
    notaContent << left << setw(20) << "No KTP" << ": " << p.noKTP << endl;
    notaContent << left << setw(20) << "No Telepon" << ": " << p.noTelp << endl;
    notaContent << "-----------------------------------------------\n";
    notaContent << left << setw(20) << "Stasiun Asal" << ": " << (indexAsal != -1 ? stasiuns[indexAsal].nama : "N/A") << endl;
    notaContent << left << setw(20) << "Stasiun Tujuan" << ": " << (indexTujuan != -1 ? stasiuns[indexTujuan].nama : "N/A") << endl;
    notaContent << left << setw(20) << "Kereta" << ": " << (indexKereta != -1 ? keretas[indexKereta].nama : "N/A") << endl;
    notaContent << left << setw(20) << "Tgl Berangkat" << ": " << p.tanggalBerangkat << endl;
    notaContent << "-----------------------------------------------\n";
    notaContent << left << setw(20) << "Jumlah Tiket" << ": " << p.jumlahTiket << endl;
    notaContent << left << setw(20) << "Total Bayar" << ": " << formatRupiah(p.totalHarga) << endl;
    notaContent << left << setw(20) << "Status" << ": " << p.status << endl;
    notaContent << left << setw(20) << "Tgl Pemesanan" << ": " << p.tanggalPesan << endl;
    notaContent << "===============================================\n";
    notaContent << "    Terima kasih telah menggunakan layanan kami\n";
    notaContent << "===============================================\n";

    cout << "\n--- PREVIEW NOTA ---\n" << notaContent.str();
    string filename = "tiket_" + to_string(id) + ".txt";
    ofstream file(filename);
    if (file.is_open()) {
        file << notaContent.str();
        file.close();
        cout << "\nNota berhasil dicetak dan disimpan sebagai '" << filename << "'" << endl;
    } else {
        cerr << "\nError: Gagal menyimpan nota ke file." << endl;
    }
    pauseScreen();
}

// ===============================================================
// FUNGSI MENU UTAMA
// ===============================================================
void tampilkanMenu() {
    int pilihan = 0;
    bool isAdmin = (users[currentUserIndex].role == "admin");

    while (pilihan != 7) {
        clearScreen();
        cout << "===============================================" << endl;
        cout << "       SISTEM MANAJEMEN TIKET KERETA API" << endl;
        cout << "===============================================" << endl;
        cout << "User: " << users[currentUserIndex].username << " | Role: " << users[currentUserIndex].role << endl;
        cout << "-----------------------------------------------" << endl;
        cout << "1. Tambah Pemesanan" << endl;
        cout << "2. Tampilkan Pemesanan" << (isAdmin ? " (Semua)" : " (Milik Saya)") << endl;
        cout << "3. Update Pemesanan" << endl;
        cout << "4. Hapus Pemesanan" << endl;
        if (isAdmin) {
            cout << "5. Dashboard Statistik (Admin Only)" << endl;
        }
        cout << "6. Cetak Nota" << endl;
        cout << "7. Logout & Keluar" << endl;
        cout << "===============================================" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pilihan = 0;
        }
        switch (pilihan) {
            case 1: tambahPemesanan(); break;
            case 2: tampilkanPemesanan(); break;
            case 3: updatePemesanan(); break;
            case 4: hapusPemesanan(); break;
            case 5: 
                if (isAdmin) dashboard();
                else {
                    cout << "\nAkses ditolak. Fitur ini hanya untuk admin." << endl;
                    pauseScreen();
                }
                break;
            case 6: cetakNota(); break;
            case 7: cout << "\nLogout berhasil. Sampai jumpa!" << endl; break;
            default:
                cout << "\nPilihan tidak valid. Silakan coba lagi." << endl;
                pauseScreen();
                break;
        }
    }
}

// ===============================================================
// FUNGSI MAIN
// ===============================================================
int main() {
    loadUsers();
    loadStasiuns();
    loadKeretas();
    loadPemesanans();

    int pilihan = 0;
    while (pilihan != 3) {
        clearScreen();
        cout << "===============================================" << endl;
        cout << "   SELAMAT DATANG DI SISTEM TIKET KERETA API" << endl;
        cout << "===============================================" << endl;
        cout << "1. Login" << endl;
        cout << "2. Buat Akun" << endl;
        cout << "3. Keluar" << endl;
        cout << "===============================================" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pilihan = 0;
        }

        switch (pilihan) {
            case 1:
                if (login()) {
                    tampilkanMenu();
                    // Reset pilihan agar kembali ke menu login setelah logout
                    pilihan = 0; 
                }
                break;
            case 2: buatAkun(); break;
            case 3:
                cout << "\nTerima kasih telah menggunakan sistem ini. Sampai jumpa!" << endl;
                return 0;
            default:
                cout << "\nPilihan tidak valid. Silakan coba lagi." << endl;
                pauseScreen();
                break;
        }
    }

    return 0;
}
