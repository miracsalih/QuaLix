#define Q "\n"
#include <complex>
#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct INFO {
    uint32_t counter = 0;
    uint32_t uinter = 0;
};

INFO info;

struct t {
    string OPCODE;        // KAPI, RAM, PORTA vs.
    bool MODE;          // IMM mi REG mi?
    string DATA1;         // 123, 0xAA (hedef)
    string DATA2;         // 456, 0xBB (kaynak)
};

t T;

// Yerel fonks:,

uint8_t reg(string reg_id) {
    if (reg_id == "AR" || reg_id == "ar") {
        return 1;
    } else if (reg_id == "BADR" || reg_id == "badr") {
        return 2;
    } else if (reg_id == "BGÇR" || reg_id == "bgçr") {
        return 3;
    } else if (reg_id == "ZR" || reg_id == "zr") {
        return 4;
    } else if (reg_id == "GÇR" || reg_id == "gçr") {
        return 5;
    } else if (reg_id == "0R" || reg_id == "0r") {
        return 6;
    } else if (reg_id == "1R" || reg_id == "1r") {
        return 7;
    } else if (reg_id == "2R" || reg_id == "2r") {
        return 8;
    } else if (reg_id == "3R" || reg_id == "3r") {
        return 9;
    } else if (reg_id == "4R" || reg_id == "4r") {
        return 10;
    } else if (reg_id == "5R" || reg_id == "5r") {
        return 11;
    } else if (reg_id == "6R" || reg_id == "6r") {
        return 12;
    } else if (reg_id == "7R" || reg_id == "7r") {
        return 13;
    } else {
        cerr << "SATIR: " << info.uinter << " Geçersiz REG ismi." << endl;
        return 0;
    }
}

#ifdef __linux__
void cls() {
    system("clear");
    return;
}

void load() {
    system("sudo cp ./QuaLix /usr/local/bin/QuaLix");
    system("sudo chmod +x /usr/local/bin/QuaLix");

    std::cout << "Kurulum tamamlandı!" << std::endl;
    return;
}
#endif

#ifdef WIN32
void cls() {
    system("cls");
    return;
}

void load() {
    cout << "Kurulum için QuaLix'e: Sağ Tık > \"Yönetici olarak çalıştır\" seçeneğine tıklayınız." << endl;
    system("mkdir \"C:\\Program Files\\QuaLix\"");
    system("copy QuaLix.exe \"C:\\Program Files\\QuaLix\"");
    system("setx PATH \"%PATH%;C:\\Program Files\\QuaLix\" /M");
}
#endif

int main(int argc, char* argv[]) {
    cls();

    if (argc < 2) {
        cerr << "Boş argüman girişi yaptınız." << endl;
        return 1;
    }

    if (argv[1] == "/load") {
        load();
    }

    string KOD;
    size_t i = 0;

    ifstream File(argv[1]);

    if (!File.is_open()) {
        cls();
        cerr << "HATA: Dosya Açılamadı!\n\nHata Kodu: 0x0002" << endl;
        return 0x0002;
    }

    if (!File) {
        cls();
        cerr << "HATA: Dosya bulunamadı!\n\nHata Kodu: 0x0003" << endl;
        return 0x0003;
    }

    if (File.fail()) {
        cls();
        cerr << "HATA: Dosyada bir hata meydana geldi! Bu hata \"Okuma\" ve \"Yazma\" veya beklenen veri türünün gelmemesi olabilir.\n\nHata Kodu: 0x0004" << endl;
        return 0x0004;
    }

    if (File.bad()) {
        cls();
        cerr << "HATA: Dosyada kritik bir hata meydana geldi!\n\nHata Kodu: 0x0005" << endl;
        return 0x0005;
    }

    if (File.good()) {
        cout << "\nDosya başarıyla açıldı & başlatıldı." << endl;
    }

    remove(argv[2]);
    ofstream Out(argv[2]);

    if (!Out.is_open()) {
        cls();
        cerr << "HATA: Çıktı dosya Açılamadı!\n\nHata Kodu: 0x0006" << endl;
        return 0x0006;
    }

    if (!Out) {
        cls();
        cerr << "HATA: Çıktı dosya bulunamadı!\n\nHata Kodu: 0x0007" << endl;
        return 0x0007;
    }

    if (Out.fail()) {
        cls();
        cerr << "HATA: Çıktı dosyada bir hata meydana geldi! Bu hata \"Okuma\" ve \"Yazma\" veya beklenen veri türünün gelmemesi olabilir.\n\nHata Kodu: 0x0008" << endl;
        return 0x0008;
    }

    if (Out.bad()) {
        cls();
        cerr << "HATA: Çıktı dosyada kritik bir hata meydana geldi!\n\nHata Kodu: 0x0009" << endl;
        return 0x0009;
    }

    if (Out.good()) {
        cout << "\nÇıktı dosya başarıyla açıldı & başlatıldı." << endl;
    }

    string c;
    string d;
    string lab[1024];
    uint16_t ip = 0;
    uint32_t imm;
    bool tab;

    while (getline(File, KOD)) {
        T.OPCODE = "";
        T.DATA1 = "";
        T.DATA2 = "";
        T.MODE = true;
        info.uinter++;
        i = 0;
        info.counter = 0;

        if (KOD.substr(0, 3) == "   ") {
            tab = true;
        } else {
            tab = false;
        }

        while (i < KOD.size()) {
            c = KOD.substr(i, 1);

            if (i + 1 < KOD.size()) {
                d = KOD.substr(i + 1, 1);
            } else {
                d = "";
            }

            if (c == " ") {
                if (d != " " && d != "," && d != "") {
                    if (info.counter == 3) {
                        break;
                    } else {
                        if (info.counter == 0 && tab == true) {
                            tab = false;
                            i++;
                            continue;
                        }
                        if (tab == false) {
                            info.counter++;
                            i++;
                            continue;
                        }
                    }
                }
            } else if (c != " ") {
                if (info.counter == 3) break;
                if (info.counter == 0) T.OPCODE += c;
                if (info.counter == 1) T.DATA1 += c;
                if (info.counter == 2) T.DATA2 += c;
            }

            i++;
        }

        if (T.OPCODE == "ETİKET" || T.OPCODE == "etiket") {
            if (T.DATA1 == "ATA" || T.DATA1 == "ata") {
                ip = 0;
                while (lab[ip] != "") {
                    ip++;
                }
                lab[ip] = T.DATA2;
                lab[ip] += "|" + to_string(info.uinter);
            }
        }
        ip++;
    }

    ip = 0;
    while (true) {
        while (lab[ip] != "") {
            ip++;
        }
        if (lab[ip] == "") {
            break;
        }
        Out << lab[ip];
    }

    info.uinter = 0;
    File.clear();
    File.seekg(0, std::ios::beg);

    while (getline(File, KOD)) {
        T.OPCODE = "";
        T.DATA1 = "";
        T.DATA2 = "";
        T.MODE = true;
        info.uinter++;
        ip = 0;
        i = 0;
        info.counter = 0;

        if (KOD.substr(0, 3) == "   ") {
            tab = true;
        } else {
            tab = false;
        }

        while (i < KOD.size()) {
            c = KOD.substr(i, 1);

            if (i + 1 < KOD.size()) {
                d = KOD.substr(i + 1, 1);
            } else {
                d = "";
            }

            if (c == " ") {
                if (d != " " && d != "," && d != "") {
                    if (info.counter == 3) {
                        break;
                    } else {
                        if (info.counter == 0 && tab == true) {
                            tab = false;
                            i++;
                            continue;
                        }
                        if (tab == false) {
                            info.counter++;
                            i++;
                            continue;
                        }
                    }
                }
            } else if (c != " ") {
                if (info.counter == 3) break;
                if (info.counter == 0) T.OPCODE += c;
                if (info.counter == 1) T.DATA1 += c;
                if (info.counter == 2) T.DATA2 += c;
            }

            i++;
        }

        if (T.DATA2.back() == 'R' || T.DATA2.back() == 'r') {
            if (reg(T.DATA2) == 0) {
                T.MODE = false;
            } else {
                T.MODE = true;
            }
        } else {
            T.MODE = false;
        }

        if (!T.MODE) {
            if (T.DATA2.substr(0, 2) == "0b") {
                T.DATA2.erase(0, 2);
                imm = stoi(T.DATA2, nullptr, 2);
            } else if (T.DATA2.substr(0, 2) == "0x") {
                T.DATA2.erase(0, 2);
                imm = stoi(T.DATA2, nullptr, 16);
            } else if (T.DATA2.substr(0, 2) == "0d") {
                T.DATA2.erase(0, 2);
                imm = stoi(T.DATA2, nullptr, 10);
            }
        } else {
            imm = 0;
        }

        if (T.OPCODE == "TAŞI" || T.OPCODE == "taşı") {
            if (T.MODE == true) {
                Out << 0x2 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|" << Q;
            } else if (T.MODE == false) {
                Out << 0x3 << "|" << (int)reg(T.DATA1) << "|" << (int)imm << "|" << Q;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "EKLE" || T.OPCODE == "ekle") {
            if (T.MODE == true) {
                Out << 0x4 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|" << Q;
            } else if (T.MODE == false) {
                Out << 0x5 << "|" << (int)reg(T.DATA1) << "|" << (int)imm << "|" << Q;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "ÇIKAR" || T.OPCODE == "çıkar") {
            if (T.MODE == true) {
                Out << 0x6 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|" << Q;
            } else if (T.MODE == false) {
                Out << 0x7 << "|" << (int)reg(T.DATA1) << "|" << (int)imm << "|" << Q;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "ARR" || T.OPCODE == "arr") {
            if (T.DATA1 == "++") {
                Out << 0x8 << "|" << (int)reg(T.DATA2) << "|" << Q;
            }
            else if (T.DATA1 == "--") {
                Out << 0x9 << "|" << (int)reg(T.DATA2) << "|" << Q;
            }
        }

        if (T.OPCODE == "BÖL" || T.OPCODE == "böl") {
            if (T.MODE == true) {
                Out << 0xA << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|" << Q;
            } else if (T.MODE == false) {
                Out << 0xB << "|" << (int)reg(T.DATA1) << "|" << (int)imm << "|" << Q;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "ÇARP" || T.OPCODE == "çarp") {
            if (T.MODE == true) {
                Out << 0xC << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|" << Q;
            } else if (T.MODE == false) {
                Out << 0xD << "|" << (int)reg(T.DATA1) << "|" << (int)imm << "|" << Q;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "EĞER" || T.OPCODE == "eğer") {
            if (T.MODE == true) {
                Out << 0xE << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|" << Q;
            } else if (T.MODE == false) {
                Out << 0xF << "|" << (int)reg(T.DATA1) << "|" << (int)imm << "|" << Q;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "BÜYÜK" || T.OPCODE == "büyük") {
            if (T.MODE == true) {
                Out << 0x10 << "|" << (int)reg(T.DATA1) << "|" << Q;
            } else if (T.MODE == false) {
                Out << 0x11 << "|" << (int)imm << "|" << Q;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "KÜÇÜK" || T.OPCODE == "küçük") {
            if (T.MODE == true) {
                Out << 0x12 << "|" << (int)reg(T.DATA1) << "|" << Q;
            } else if (T.MODE == false) {
                Out << 0x13 << "|" << (int)imm << "|" << Q;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "DOĞRU" || T.OPCODE == "doğru") {
            if (T.MODE == true) {
                Out << 0x14 << "|" << (int)reg(T.DATA1) << "|" << Q;
            } else if (T.MODE == false) {
                Out << 0x15 << "|" << (int)imm << "|" << Q;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "YANLIŞ" || T.OPCODE == "yanlış") {
            if (T.MODE == true) {
                Out << 0x16 << "|" << (int)reg(T.DATA1) << "|" << Q;
            } else if (T.MODE == false) {
                Out << 0x17 << "|" << (int)imm << "|" << Q;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "ZIPLA" || T.OPCODE == "zıpla") {
            if (T.DATA1 == "OTO" || T.DATA1 == "oto") {
                bool found = false;
                ip = 0;
                while (lab[ip] != "") {
                    i = lab[ip].find('|');
                    size_t p = lab[ip].size();
                    if (i == string::npos || lab[ip].substr(0, i) != T.DATA2) {
                        ip++;
                        continue;
                    }
                    string satirNo = lab[ip].substr(i + 1, p - i);
                    Out << 0x18 << "|" << stoi(satirNo, nullptr, 10) << "|" << Q;
                    found = true;
                    break;
                }
                if (!found) {
                    cerr << "SATIR: " << info.uinter << " Etiket bulunamadı: " << T.DATA2 << endl;
                    continue;
                }
            } else {
                continue;
            }
        }

        if (T.OPCODE == "REB" || T.OPCODE == "reb") {
            if (T.DATA1 == "YAZ" || T.DATA1 == "yaz") {
                Out << 0x19 << "|" << Q;
            } else if (T.DATA1 == "OKU" || T.DATA1 == "oku") {
                Out << 0x1A << "|" << Q;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "PORT" || T.OPCODE == "port") {
            if (T.DATA1 == "YAZ" || T.DATA1 == "yaz") {
                if (T.MODE == true) {
                    Out << 0x1B << "|" << (int)reg(T.DATA2) << "|" << Q;
                } else if (T.MODE == false) {
                    Out << 0x1C << "|" << (int)imm << "|" << Q;
                } else {
                    continue;
                }
            } else if (T.DATA1 == "OKU" || T.DATA1 == "oku") {
                if (T.MODE == true) {
                    Out << 0x1D << "|" << (int)reg(T.DATA2) << "|" << Q;
                } else if (T.MODE == false) {
                    Out << 0x1E << "|" << (int)imm << "|" << Q;
                } else {
                    continue;
                }
            } else {
                continue;
            }
        }

        if (T.OPCODE == "VE" || T.OPCODE == "ve") {
            if (T.MODE == true) {
                Out << 0x1F << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|" << Q;
            } else if (T.MODE == false) {
                Out << 0x20 << "|" << (int)reg(T.DATA1) << "|" << (int)imm << "|" << Q;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "VEYA" || T.OPCODE == "veya") {
            if (T.MODE == true) {
                Out << 0x21 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|" << Q;
            } else if (T.MODE == false) {
                Out << 0x22 << "|" << (int)reg(T.DATA1) << "|" << (int)imm << "|" << Q;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "DEĞİL" || T.OPCODE == "değil") {
            if (T.MODE == true) {
                Out << 0x23 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|" << Q;
            } else if (T.MODE == false) {
                Out << 0x24 << "|" << (int)reg(T.DATA1) << "|" << (int)imm << "|" << Q;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "IRQ" || T.OPCODE == "irq") {
            if (T.DATA1 == "OTO" || T.DATA1 == "oto") {
                bool found = false;
                ip = 0;
                while (lab[ip] != "") {
                    i = lab[ip].find('|');
                    size_t p = lab[ip].size();
                    if (i == string::npos || lab[ip].substr(0, i) != T.DATA2) {
                        ip++;
                        continue;
                    }
                    string satirNo = lab[ip].substr(i + 1, p - i);
                    Out << 0x25 << "|" << stoi(satirNo, nullptr, 10) << "|" << Q;
                    found = true;
                    break;
                }
                if (!found) {
                    cerr << "SATIR: " << info.uinter << " Etiket bulunamadı: " << T.DATA2 << endl;
                    continue;
                }
            } else {
                continue;
            }
        }
    }
}
