#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <string>
#include <conio.h>

using namespace std;
// deklarasi maksimal lokasi
const int MAX_LOCATIONS = 100;
// deklarasi void menjadi global
void login();
void daftar();

struct Location {
  string name;
  string address;
  int connections[MAX_LOCATIONS];
  int weights[MAX_LOCATIONS];
  int numConnections;
};

Location locations[MAX_LOCATIONS];
int numLocations = 0;
// Menambahkan lokasi ke dalam queue
void addLocation(string fileName) {
  if (numLocations >= MAX_LOCATIONS) {
    cout << "Kapasitas lokasi penuh." << endl;
    return;
  }

  cout << "Lokasi ke-" << numLocations + 1 << endl;
  Location location;

  cout << "Masukkan nama lokasi: ";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  getline(cin, location.name);

  cout << "Masukkan alamat lokasi: ";
  getline(cin, location.address);

  location.numConnections = 0;

  locations[numLocations] = location;
  numLocations++;

  cout << "Lokasi berhasil ditambahkan!" << endl;
  // menambahkan data lokasi ke dalam txt
  ifstream inFile(fileName.c_str());
  ofstream outFile("temp.txt", ios::app);
  // memasukkan file kedalam txt
  if (inFile.is_open() && outFile.is_open()) {
    string line;

    while (getline(inFile, line)) {
      if (line == "Koneksi Berarah") {
          outFile << location.name << endl;
          outFile << location.address << endl;
      }
      outFile << line << endl;
    }

    inFile.close();
    outFile.close();

    remove(fileName.c_str());
    rename("temp.txt", fileName.c_str());
  } else {
    cout << "Gagal membuka file." << endl;
  }
}
// Menampilkan lokasi yang sudah ada
void printLocations() {
  cout << "Daftar Lokasi:" << endl;
  for (int i = 0; i < numLocations; i++) {
    cout << "Lokasi " << i + 1 << ":" << endl;
    cout << "Nama: " << locations[i].name << endl;
    cout << "Alamat: " << locations[i].address << endl;
    cout << endl;
  }
}
// Menambahkan koneksi antara lokasi yang sudah ada
void connectLocations(string fileName) {
  if (numLocations < 2) {
    cout << "Tidak ada cukup lokasi untuk melakukan koneksi." << endl;
    return;
  }

  int source, target, weight;
  cout << "Pilih sumber lokasi [1-" << numLocations << "]: ";
  cin >> source;

  if (source < 1 || source > numLocations) {
    cout << "Indeks sumber lokasi tidak valid." << endl;
    return;
  }

  cout << "Pilih target lokasi [1-" << numLocations << "]: ";
  cin >> target;

  if (target < 1 || target > numLocations) {
    cout << "Indeks target lokasi tidak valid." << endl;
    return;
  }

  if (source == target) {
    cout << "Sumber dan target lokasi sama." << endl;
    return;
  }
  // Periksa apakah sumber dan target sudah terhubung
  bool isConnected = false;
  for (int i = 0; i < locations[source - 1].numConnections; i++) {
    if (locations[source - 1].connections[i] == target - 1) {
      cout << "Lokasi " << source << " dan Lokasi " << target << " sudah terhubung." << endl;
      cout << "Jarak: " << locations[source - 1].weights[i] << "km" << endl;
      isConnected = true;
      break;
    }
  }

  if (!isConnected) {
    penjaluran :
    string jalur;
    cout << "Apakah ini satu jalur?[y/t]: ";
    cin >> jalur;
    
    if (jalur == "y" || jalur == "Y") {
      cout << "Masukkan jarak: ";
      cin >> weight;
      locations[source - 1].connections[locations[source - 1].numConnections] = target - 1;
      locations[source - 1].weights[locations[source - 1].numConnections] = weight;
      locations[source - 1].numConnections++;

      // menambahkan data koneksi berarah ke dalam txt
      ifstream inFile(fileName.c_str());
      ofstream outFile("temp.txt", ios::app);
      // memasukkan file kedalam txt
      if (inFile.is_open() && outFile.is_open()) {
        string line;

        while (getline(inFile, line)) {
          if (line == "Koneksi Tak Berarah") {
              outFile << source - 1 << " " << target - 1 << " " << weight << endl;
          }
          outFile << line << endl;
        }

        inFile.close();
        outFile.close();

        remove(fileName.c_str());
        rename("temp.txt", fileName.c_str());
      } else {
        cout << "Gagal membuka file." << endl;
      }
    }
    else if (jalur == "t" || jalur == "T") {
      cout << "Masukkan jarak: ";
      cin >> weight;
      locations[source - 1].connections[locations[source - 1].numConnections] = target - 1;
      locations[source - 1].weights[locations[source - 1].numConnections] = weight;
      locations[source - 1].numConnections++;

      locations[target - 1].connections[locations[target - 1].numConnections] = source - 1;
      locations[target - 1].weights[locations[target - 1].numConnections] = weight;
      locations[target - 1].numConnections++;

      // menambahkan data koneksi tak berarah ke dalam txt
      ofstream outFile(fileName.c_str(), ios::app);

      if (outFile.is_open()) {
        outFile << source - 1 << " " << target - 1 << " " << weight << endl;
        outFile.close();
      } else {
        cout << "Gagal membuka file." << endl;
      }
    }
    else {
      cout << "Silahkan input dengan benar! \n" << endl;
      goto penjaluran;
    }
    cout << "Lokasi " << source << " dan Lokasi " << target << " berhasil terhubung dengan jarak " << weight << "km." << endl;
  }
}
// Algoritma Dijkstra untuk mencari jalur terpendek
void dijkstra(int source, int target) {
  int distance[MAX_LOCATIONS];
  int parent[MAX_LOCATIONS];

  for (int i = 0; i < numLocations; i++) {
    distance[i] = numeric_limits<int>::max();
    parent[i] = -1;
  }

  distance[source] = 0;

  priority_queue<pair<int, int>, vector<pair<int, int>>,
                 greater<pair<int, int>>> pq;
  pq.push(make_pair(0, source));

  while (!pq.empty()) {
    int u = pq.top().second;
    pq.pop();

    for (int i = 0; i < locations[u].numConnections; i++) {
      int v = locations[u].connections[i];
      int weight = locations[u].weights[i];

      if (distance[v] > distance[u] + weight) {
        distance[v] = distance[u] + weight;
        parent[v] = u;
        pq.push(make_pair(distance[v], v));
      }
    }
  }

  if (distance[target] == numeric_limits<int>::max()) {
    cout << "Tidak ada jalur yang tersedia." << endl;
    return;
  }

  cout << "Jalur terpendek dari " << locations[source].name << " ke "
       << locations[target].name << ":" << endl;

  int current = target;
  vector<int> path;

  while (current != -1) {
    path.push_back(current);
    current = parent[current];
  }

  // Menampilkan jalur terpendek
  for (int i = path.size() - 1; i >= 0; i--) {
    cout << locations[path[i]].name;
    if (i > 0) {
      cout << " -> ";
    }
  }

  cout << endl;
  cout << "Jarak yang ditempuh: " << distance[target] << "km" << endl;
}
// Mencari jalur terpendek antara dua lokasi
void shortestPath(string fileName) {
  if (numLocations < 2) {
    cout << "Tidak ada cukup lokasi untuk mencari jalur terpendek." << endl;
    return;
  }

  int source, target;
  cout << "Pilih lokasi asal [1-" << numLocations << "]: ";
  cin >> source;

  if (source < 1 || source > numLocations) {
    cout << "Indeks lokasi asal tidak valid." << endl;
    return;
  }

  cout << "Pilih lokasi tujuan [1-" << numLocations << "]: ";
  cin >> target;

  if (target < 1 || target > numLocations) {
    cout << "Indeks lokasi tujuan tidak valid." << endl;
    return;
  }

  source--;
  target--;
  // setelah memilih lokasi asal dan tujuan, lalu jalankan algoritma dijkstra
  dijkstra(source, target);
}
// Memuat data lokasi dari file
void loadData(const string &fileName) {
  ifstream inFile(fileName);

  if (inFile.is_open()) {
    numLocations = 0;

    string line, name, address;
    // pengecekan apakah sedang membaca bagian lokasi atau koneksi
    bool isLocationSection = false;
    bool isDirectionalConnectionSection = false;
    bool isPointlessConnectionSection = false;

    while (getline(inFile, line)) {
      if (line == "Lokasi") {
        isLocationSection = true;
        isDirectionalConnectionSection = false;
        isPointlessConnectionSection = false;
        continue;
      } 
      else if (line == "Koneksi Berarah") {
        isLocationSection = false;
        isDirectionalConnectionSection = true;
        isPointlessConnectionSection = false;
        continue;
      }
      else if (line == "Koneksi Tak Berarah") {
        isLocationSection = false;
        isDirectionalConnectionSection = false;
        isPointlessConnectionSection = true;
        continue;
      }

      if (isLocationSection) {
        if (name.empty()) {
          name = line;
        }
        else {
          address = line;
          Location location;
          location.name = name;
          location.address = address;
          location.numConnections = 0;
          locations[numLocations] = location;
          numLocations++;
          name.clear();
          address.clear();
        }
      } 
      else if (isDirectionalConnectionSection) {
        istringstream iss(line);
        int source, target, weight;
        iss >> source >> target >> weight;
        locations[source].connections[locations[source].numConnections] = target;
        locations[source].weights[locations[source].numConnections] = weight;
        locations[source].numConnections++;
      }
      else if (isPointlessConnectionSection) {
        istringstream iss(line);
        int source, target, weight;
        iss >> source >> target >> weight;
        locations[source].connections[locations[source].numConnections] = target;
        locations[source].weights[locations[source].numConnections] = weight;
        locations[source].numConnections++;
        locations[target].connections[locations[target].numConnections] = source;
        locations[target].weights[locations[target].numConnections] = weight;
        locations[target].numConnections++;
      }
    }

    inFile.close();
    cout << "Data berhasil dimuat." << endl;
  } else {
    cout << "Gagal membuka file." << endl;
  }
}
// menghapus lokasi
void deleteLocation(string fileName) {
  if (numLocations == 0) {
    cout << "Tidak ada lokasi yang tersedia." << endl;
    return;
  }

  int index;
  cout << "Pilih lokasi yang ingin dihapus [1-" << numLocations << "]: ";
  cin >> index;

  if (index < 1 || index > numLocations) {
    cout << "Indeks lokasi tidak valid." << endl;
    return;
  }

  index--;
  // Menghapus koneksi yang terhubung dengan lokasi yang dihapus
  for (int i = 0; i < numLocations; i++) {
    for (int j = 0; j < locations[i].numConnections; j++) {
      if (locations[i].connections[j] == index) {
        // Geser koneksi ke kiri
        for (int k = j; k < locations[i].numConnections - 1; k++) {
          locations[i].connections[k] = locations[i].connections[k + 1];
          locations[i].weights[k] = locations[i].weights[k + 1];
        }
        locations[i].numConnections--;
        j--; // Penyesuaian indeks setelah penghapusan
        // Perbarui indeks koneksi pada lokasi 1 jika diperlukan
        if (locations[i].connections[j] > index) {
          locations[i].connections[j]--; // Kurangi indeks
        }
      }
    }
  }

  for (int i = index; i < numLocations - 1; i++) {
    locations[i] = locations[i + 1];
  }

  numLocations--;
  // Menulis ulang data ke file
  ofstream outFile(fileName.c_str(), ios::trunc);

  if (outFile.is_open()) {
    outFile << "Lokasi" << endl;
    for (int i = 0; i < numLocations; i++) {
      outFile << locations[i].name << endl;
      outFile << locations[i].address << endl;
    }

    outFile << "Koneksi Berarah" << endl;
    for (int i = 0; i < numLocations; i++) {
      for (int j = 0; j < locations[i].numConnections; j++) {
        int target = locations[i].connections[j];
        int weight = locations[i].weights[j];

        // Cek apakah koneksi berarah
        bool isDirected = false;
        for (int k = 0; k < locations[target].numConnections; k++) {
          if (locations[target].connections[k] == i && locations[target].weights[k] == weight) {
            isDirected = true;
            break;
          }
        }

        if (!isDirected) {
          outFile << i << " " << target << " " << weight << endl;
        }
      }
    }

    outFile << "Koneksi Tak Berarah" << endl;
    for (int i = 0; i < numLocations; i++) {
      for (int j = 0; j < locations[i].numConnections; j++) {
        int target = locations[i].connections[j];
        int weight = locations[i].weights[j];

        // Cek apakah koneksi tak berarah
        bool isUndirected = false;
        for (int k = 0; k < locations[target].numConnections; k++) {
          if (locations[target].connections[k] == i && locations[target].weights[k] == weight) {
            isUndirected = true;
            break;
          }
        }

        bool connectionExists = false;
        for (int k = 0; k < i; k++) {
          if (locations[k].connections[j] == i && locations[i].connections[j] == k && locations[i].weights[j] == locations[k].weights[j]) {
            connectionExists = true;
            break;
          }
        }

        if (isUndirected && connectionExists) {
          outFile << i << " " << target << " " << weight << endl;
        }
      }
    }

    outFile.close();
    cout << "Lokasi berhasil dihapus!" << endl;
  } 
  else {
  cout << "Gagal membuka file." << endl;
  }
}
// mengedit bobot lokasi yang sudah ada
void changeWeight(string fileName) {
  if (numLocations < 2) {
    cout << "Tidak ada cukup lokasi untuk melakukan perubahan jarak." << endl;
    return;
  }

  int source, target, weight;
  cout << "Pilih sumber lokasi [1-" << numLocations << "]: ";
  cin >> source;

  if (source < 1 || source > numLocations) {
    cout << "Indeks sumber lokasi tidak valid." << endl;
    return;
  }

  cout << "Pilih target lokasi [1-" << numLocations << "]: ";
  cin >> target;

  if (target < 1 || target > numLocations) {
    cout << "Indeks target lokasi tidak valid." << endl;
    return;
  }

  if (source == target) {
    cout << "Sumber dan target lokasi sama." << endl;
    return;
  }

  cout << "Masukkan jarak baru: ";
  cin >> weight;
  // mengecek apakah ada koneksi antara sumber dan target
  bool foundConnection = false;

  for (int i = 0; i < locations[source - 1].numConnections; i++) {
    if (locations[source - 1].connections[i] == target - 1) {
      locations[source - 1].weights[i] = weight;
      foundConnection = true;
      break;
    }
  }

  if (!foundConnection) {
    cout << "Tidak ditemukan koneksi antara sumber dan target lokasi." << endl;
    return;
  }

  for (int i = 0; i < locations[target - 1].numConnections; i++) {
    if (locations[target - 1].connections[i] == source - 1) {
      locations[target - 1].weights[i] = weight;
      break;
    }
  }
  // menulis ulang data ke file
  ofstream outFile(fileName.c_str(), ios::trunc);

  if (outFile.is_open()) {
    outFile << "Lokasi" << endl;
    for (int i = 0; i < numLocations; i++) {
      outFile << locations[i].name << endl;
      outFile << locations[i].address << endl;
    }

    outFile << "Koneksi Berarah" << endl;
    for (int i = 0; i < numLocations; i++) {
      for (int j = 0; j < locations[i].numConnections; j++) {
        int target = locations[i].connections[j];
        int weight = locations[i].weights[j];

        // Cek apakah koneksi berarah
        bool isDirected = false;
        for (int k = 0; k < locations[target].numConnections; k++) {
          if (locations[target].connections[k] == i && locations[target].weights[k] == weight) {
            isDirected = true;
            break;
          }
        }

        if (!isDirected) {
          outFile << i << " " << target << " " << weight << endl;
        }
      }
    }

    outFile << "Koneksi Tak Berarah" << endl;
    for (int i = 0; i < numLocations; i++) {
      for (int j = 0; j < locations[i].numConnections; j++) {
        int target = locations[i].connections[j];
        int weight = locations[i].weights[j];

        // Cek apakah koneksi tak berarah
        bool isUndirected = false;
        for (int k = 0; k < locations[target].numConnections; k++) {
          if (locations[target].connections[k] == i && locations[target].weights[k] == weight) {
            isUndirected = true;
            break;
          }
        }

        bool connectionExists = false;
        for (int k = 0; k < i; k++) {
          if (locations[k].connections[j] == i && locations[i].connections[j] == k && locations[i].weights[j] == locations[k].weights[j]) {
            connectionExists = true;
            break;
          }
        }

        if (isUndirected && connectionExists) {
          outFile << i << " " << target << " " << weight << endl;
        }
      }
    }

    outFile.close();
    cout << "Jarak berhasil diubah!" << endl;
  } else {
    cout << "Gagal membuka file." << endl;
  }
}

// mengedit lokasi yang sudah ada
void editLocation(string fileName) {
  if (numLocations == 0) {
    cout << "Tidak ada lokasi yang tersedia." << endl;
    return;
  }

  int index;
  cout << "Pilih lokasi yang ingin diubah [1-" << numLocations << "]: ";
  cin >> index;

  if (index < 1 || index > numLocations) {
    cout << "Indeks lokasi tidak valid." << endl;
    return;
  }

  index--;

  int pilihan;
  cout << "Pilih data yang ingin diubah:" << endl;
  cout << "[1] Nama Lokasi" << endl;
  cout << "[2] Alamat Lokasi" << endl;
  cout << "Silahkan pilih opsi [1-2]: ";
  cin >> pilihan;

  if (pilihan == 1) {
    cout << "Masukkan nama lokasi baru: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, locations[index].name);
    cout << "Nama lokasi berhasil diubah!" << endl;
  } else if (pilihan == 2) {
    cout << "Masukkan alamat lokasi baru: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, locations[index].address);
    cout << "Alamat lokasi berhasil diubah!" << endl;
  } else {
    cout << "Opsi tidak valid." << endl;
    return;
  }
  // menulis ulang data ke file
  ofstream outFile(fileName.c_str(), ios::trunc);

  if (outFile.is_open()) {
    outFile << "Lokasi" << endl;
    for (int i = 0; i < numLocations; i++) {
      outFile << locations[i].name << endl;
      outFile << locations[i].address << endl;
    }

    outFile << "Koneksi Berarah" << endl;
    for (int i = 0; i < numLocations; i++) {
      for (int j = 0; j < locations[i].numConnections; j++) {
        int target = locations[i].connections[j];
        int weight = locations[i].weights[j];

        // Cek apakah koneksi berarah
        bool isDirected = false;
        for (int k = 0; k < locations[target].numConnections; k++) {
          if (locations[target].connections[k] == i && locations[target].weights[k] == weight) {
            isDirected = true;
            break;
          }
        }

        if (!isDirected) {
          outFile << i << " " << target << " " << weight << endl;
        }
      }
    }

    outFile << "Koneksi Tak Berarah" << endl;
    for (int i = 0; i < numLocations; i++) {
      for (int j = 0; j < locations[i].numConnections; j++) {
        int target = locations[i].connections[j];
        int weight = locations[i].weights[j];

        // Cek apakah koneksi tak berarah
        bool isUndirected = false;
        for (int k = 0; k < locations[target].numConnections; k++) {
          if (locations[target].connections[k] == i && locations[target].weights[k] == weight) {
            isUndirected = true;
            break;
          }
        }

        bool connectionExists = false;
        for (int k = 0; k < i; k++) {
          if (locations[k].connections[j] == i && locations[i].connections[j] == k && locations[i].weights[j] == locations[k].weights[j]) {
            connectionExists = true;
            break;
          }
        }

        if (isUndirected && connectionExists) {
          outFile << i << " " << target << " " << weight << endl;
        }
      }
    }

    outFile.close();
  } else {
    cout << "Gagal membuka file." << endl;
  }
}
// mengedit data lokasi dan koneksi yang sudah ada sebelumnya
void editData(string fileName) {
  
  bool backToMenu = false;
  int pilih, opsi;
  do {
  home:
    // menu edit data
    system ("cls");
    cout << "===============================================" << endl;
    cout << "                   EDIT DATA                   " << endl;
    cout << "===============================================" << endl;
    cout << "[1] Ubah nama dan alamat lokasi" << endl;
    cout << "[2] Ubah jarak antar lokasi" << endl;
    cout << "[3] Kembali ke menu utama" << endl;
    cout << "===============================================" << endl;
    cout << "Silahkan pilih menu [1-3]: ";
    cin >> pilih;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Silahkan input dengan benar!" << endl;
      system("pause");
      goto home;
    }
    
    switch (pilih) {
    case 1:
      printLocations();
      editLocation(fileName);
      goto akhir;
      break;
  
    case 2:
     printLocations();
     changeWeight(fileName);
     goto akhir;
     break;

    case 3:
     goto last;
     break;

    default:
      cout << "Silahkan input dengan benar!" << endl;
      goto home;
      break;
   }

    akhir:
    cout << "===============================================" << endl;
    cout << "[1] Kembali ke menu awal" << endl;
    cout << "[2] Ubah data lagi" << endl;
    cout << "Silahkan pilih opsi [1-2]: ";
    cin >> opsi;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Silahkan input dengan benar!" << endl;
      system("pause");
      goto akhir;
    }

   if (opsi == 1) {
    backToMenu = true;
    }

    else if (opsi == 2) {
    goto home;
    }

   else {
    cout << "Silahkan input dengan benar!" << endl;
    goto akhir;
    }

    last:
    backToMenu = true;
  } while (!backToMenu);
}
// menu utama
void menu(string username) {
  int pilihan, opsi;
  string optLogout, fileName = username + ".txt";
home:
  system("cls");
  loadData(fileName);
  cout << "=====================================================" << endl;
  cout << "                                                     " << endl;
  cout << "               Selamat Datang di MyMaps              " << endl;
  cout << "                                                     " << endl;
  cout << "=====================================================" << endl;
  cout << "[1] Input Lokasi" << endl;
  cout << "[2] Lihat Lokasi" << endl;
  cout << "[3] Hubungkan Lokasi" << endl;
  cout << "[4] Cari Rute Terpendek" << endl;
  cout << "[5] Hapus Lokasi" << endl;
  cout << "[6] Edit Data" << endl;
  cout << "[7] Logout" << endl;
  cout << "[8] Exit" << endl;
  cout << "=====================================================" << endl;
  cout << "Silahkan pilih menu [1-8]: ";
  cin >> pilihan;
  if (cin.fail()) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Silahkan input dengan benar!" << endl;
    system("pause");
    goto home;
  }

  switch (pilihan) {
  case 1:
    addLocation(fileName);
    goto akhir;
    break;

  case 2:
    printLocations();
    goto akhir;
    break;

  case 3:
    printLocations();
    connectLocations(fileName);
    goto akhir;
    break;

  case 4: {
    printLocations();
    shortestPath(fileName);
    goto akhir;
    break;
  }

  case 5:
    printLocations();
    deleteLocation(fileName);
    goto akhir;
    break;

  case 6:
    editData(fileName);
    goto home;
    break;

  case 7:
  logout:
    system("cls");
    cout << "Apakah anda yakin ingin logout?(y/t): ";
    cin >> optLogout;
    if (optLogout == "y" || optLogout == "Y") {
      login();
    } else if (optLogout == "t" || optLogout == "T") {
      goto home;
    } else {
      cout << "Silahkan input dengan benar!" << endl;
      goto logout;
    }
    break;

  case 8:
    goto end;
    break;

  default:
    cout << "Silahkan input dengan benar!" << endl;
    goto home;
    break;
  }

akhir:
  cout << "=====================================================" << endl;
  cout << "[1] Kembali ke menu awal" << endl;
  cout << "[2] Exit Program" << endl;
  cout << "Silahkan pilih opsi [1-2]: ";
  cin >> opsi;
  if (cin.fail()) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Silahkan input dengan benar!" << endl;
    system("pause");
    goto akhir;
  }

  if (opsi == 1) {
    goto home;
  }

  else if (opsi == 2) {
    goto end;
  }

  else {
    cout << "Silahkan input dengan benar!" << endl;
    goto akhir;
  }

end:
  abort();
}
// halaman login
void login() {
  string username, password, fileUsn, filePass;
  int i;
  char ch;

  for (i = 2; i >= 0; i--) {
    bool found = false;
    system("cls");
    cout << "=====================================================" << endl;
    cout << "               Selamat Datang di MyMaps              " << endl;
    cout << "                                                     " << endl;
    cout << "                   Silahkan Login                    " << endl;
    cout << "=====================================================" << endl;
    cout << "Masukkan username: ";
    cin >> username;
    cout << "Masukkan password: ";
    ch = _getch();
    while (ch != 13) {
      if (ch == 8) {
        if (password.size() > 0) {
          password.pop_back();
          cout << "\b \b";
        }
      } else {
        password.push_back(ch);
        cout << "*";
      }
      ch = _getch();
    }

    ifstream infile("accounts.txt");

    if (infile.is_open()) {
      while (infile >> fileUsn >> filePass) {
        if (username == fileUsn && password == filePass) {
          found = true;
          break;
        }
      }

      infile.close();

      if (found) {
        system("cls");
        cout << "Login Berhasil" << endl;
        system("pause");
        menu(username);
        return;
      }
    }

    if (i > 0) {
      system("cls");
      cout << "Username atau Password salah!" << endl;
      cout << "Kesempatan anda tersisa " << i << " lagi" << endl;
      password.clear();
      system("pause");
    }
  }

  system("cls");
  cout << "Anda tidak dapat mengakses aplikasi ini!" << endl;
}
// halaman daftar
void daftar() {
  string username, password, verify;
  char ch;

  system("cls");

  cout << "=====================================================" << endl;
  cout << "               Selamat Datang di MyMaps              " << endl;
  cout << "                                                     " << endl;
  cout << "                   Silahkan Daftar                   " << endl;
  cout << "=====================================================" << endl;
  cout << "Masukan username: ";
  cin >> username;

  ifstream infile("accounts.txt");
  string existingUsername;
  bool usernameExist = false;
  while (infile >> existingUsername) {
    if (username == existingUsername) {
      usernameExist = true;
      break;
    }
  }

  if (usernameExist) {
    system("cls");
    cout << "Username sudah terdaftar, silahkan gunakan username yang lain" << endl;
    system("pause");
    return daftar();
  }

  cout << "Masukan password: ";
  ch = _getch();
  while (ch != 13) {
    if (ch == 8) {
      if (password.size() > 0) {
        password.pop_back();
        cout << "\b \b";
      }
    }

    else {
      password.push_back(ch);
      cout << "*";
    }
    ch = _getch();
  }
  cout << endl;

  cout << "Verifikasi password anda: ";
  ch = _getch();
  while (ch != 13) {
    if (ch == 8) {
      if (verify.size() > 0) {
        verify.pop_back();
        cout << "\b \b";
      }

    }

    else {
      verify.push_back(ch);
      cout << "*";
    }
    ch = _getch();
  }
  cout << endl;
  // membuat file akun secara otomatis ketika berhasil mendaftar
  if (password == verify) {
    string fileName = username + ".txt";
    ofstream file(fileName.c_str(), ios::app);
    if (file.is_open()) {
      file << "Lokasi" << endl;
      file << "Koneksi Berarah" << endl;
      file << "Koneksi Tak Berarah" << endl;
      file.close();
    }
    // membuat file akun
    ofstream outfile("accounts.txt", ios::app);

    if (outfile.is_open()) {
      outfile << username << " " << password << endl;

      outfile.close();

      system("cls");
      cout << "Akun anda telah berhasil dibuat" << endl;
      system("pause");
      return login();
    }

    else {
      system("cls");
      cout << "File Error" << endl;
      system("pause");
    }
  }

  else {
    system("cls");
    cout << "Silahkan verifikasi password anda dengan benar" << endl;
    system("pause");
    return daftar();
  }
}

int main() {
  system("cls");
  string pilih;
  cout << "=====================================================" << endl;
  cout << "               Selamat Datang di MyMaps              " << endl;
  cout << "                                                     " << endl;
  cout << "                   Silahkan Login                    " << endl;
  cout << "=====================================================" << endl;
  cout << "Apakah anda sudah mempunyai akun?(y/t) ";
  cin >> pilih;
  if (pilih == "y" || pilih == "Y") {
    login();
  }

  else if (pilih == "t" || pilih == "T") {
    daftar();
  }

  else {
    cout << "Inputan Salah!" << endl;
    system("pause");
    return main();
  }
  return 0;
}