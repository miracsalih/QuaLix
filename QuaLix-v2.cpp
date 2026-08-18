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
        return 0;
    }
}

int main(int argc, char* argv[]) {
    system("clear");
    string cmd = argv[1]; // *.tbdd / command
    if (argc >= 2) string cmd2 = argv[2]; // *.trbin

    if (cmd == "/load") {
        system("sudo cp ./QuaLix /usr/local/bin/QuaLix");

        // İzin ver
        system("sudo chmod +x /usr/local/bin/QuaLix");

        std::cout << "Kurulum tamamlandı!" << std::endl;
        return 0;
    }

    string KOD;
    string KOD2;
    size_t i = 0;

    ifstream File(argv[1]);

    if (!File.is_open()) {
        system("clear");
        cout << "HATA: Dosya Açılamadı!\n\nHata Kodu: 0x0002" << endl;
        return 0x0002;
    }

    if (!File) {
        system("clear");
        cout << "HATA: Dosya bulunamadı!\n\nHata Kodu: 0x0003" << endl;
        return 0x0003;
    }

    if (File.fail()) {
        system("clear");
        cout << "HATA: Dosyada bir hata meydana geldi! Bu hata \"Okuma\" ve \"Yazma\" veya beklenen veri türünün gelmemesi olabilir.\n\nHata Kodu: 0x0004" << endl;
        return 0x0004;
    }

    if (File.bad()) {
        system("clear");
        cout << "HATA: Dosyada kritik bir hata meydana geldi!\n\nHata Kodu: 0x0005" << endl;
        return 0x0005;
    }

    if (File.good()) {
        cout << "\nDosya başarıyla açıldı & başlatıldı." << endl;
    }

    remove(argv[2]);
    ofstream Out(argv[2]);

    if (!Out.is_open()) {
        system("clear");
        cout << "HATA: Çıktı dosya Açılamadı!\n\nHata Kodu: 0x0006" << endl;
        return 0x0006;
    }

    if (!Out) {
        system("clear");
        cout << "HATA: Çıktı dosya bulunamadı!\n\nHata Kodu: 0x0007" << endl;
        return 0x0007;
    }

    if (Out.fail()) {
        system("clear");
        cout << "HATA: Çıktı dosyada bir hata meydana geldi! Bu hata \"Okuma\" ve \"Yazma\" veya beklenen veri türünün gelmemesi olabilir.\n\nHata Kodu: 0x0008" << endl;
        return 0x0008;
    }

    if (Out.bad()) {
        system("clear");
        cout << "HATA: Çıktı dosyada kritik bir hata meydana geldi!\n\nHata Kodu: 0x0009" << endl;
        return 0x0009;
    }

    if (Out.good()) {
        cout << "\nÇıktı dosya başarıyla açıldı & başlatıldı." << endl;
    }

    string c;
    string d;
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

        if (!T.DATA2.empty() && (T.DATA2.back() == 'r' || T.DATA2.back() == 'R')) {
            T.MODE = true;
        } else {
            T.MODE = false;
        }

        uint32_t immh = 0;
        uint32_t imml = 0;
        if (T.MODE == false) {
            if (T.DATA1.back() != 'r' && T.DATA1.back() != 'R') {
                if (T.DATA1.substr(0, 2) == "0b") {
                    T.DATA1.erase(0, 2);
                    imml = stoi(T.DATA1, nullptr, 2);
                } else if (T.DATA1.substr(0, 2) == "0x") {
                    T.DATA1.erase(0, 2);
                    imml = stoi(T.DATA1, nullptr, 16);
                } else {
                    imml = stoi(T.DATA1, nullptr, 10);
                }
            } else {
                if (T.DATA2.substr(0, 2) == "0b") {
                    T.DATA2.erase(0, 2);
                    immh = stoi(T.DATA2, nullptr, 2);
                } else if (T.DATA2.substr(0, 2) == "0x") {
                    T.DATA2.erase(0, 2);
                    immh = stoi(T.DATA2, nullptr, 16);
                } else {
                    immh = stoi(T.DATA2, nullptr, 10);
                }
            }
        }

        if (T.OPCODE == "TAŞI" || T.OPCODE == "taşı") {
            if (T.MODE == true) {
                Out << 0x2 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2);
            } else if (T.MODE == false) {
                Out << 0x3 << "|" << (int)reg(T.DATA1) << "|" << (int)immh;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "EKLE" || T.OPCODE == "ekle") {
            if (T.MODE == true) {
                Out << 0x4 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2);
            } else if (T.MODE == false) {
                Out << 0x5 << "|" << (int)reg(T.DATA1) << "|" << (int)immh;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "ÇIKAR" || T.OPCODE == "çıkar") {
            if (T.MODE == true) {
                Out << 0x6 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2);
            } else if (T.MODE == false) {
                Out << 0x7 << "|" << (int)reg(T.DATA1) << "|" << (int)immh;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "--") {
            Out << 0x8 << "|" << (int)reg(T.DATA1);
        }

        if (T.OPCODE == "++") {
            Out << 0xA << "|" << (int)reg(T.DATA1);
        }

        if (T.OPCODE == "BÖL" || T.OPCODE == "böl") {
            if (T.MODE == true) {
                Out << 0xC << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2);
            } else if (T.MODE == false) {
                Out << 0xD << "|" << (int)reg(T.DATA1) << "|" << (int)immh;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "ÇARP" || T.OPCODE == "çarp") {
            if (T.MODE == true) {
                Out << 0xE << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2);
            } else if (T.MODE == false) {
                Out << 0xF << "|" << (int)reg(T.DATA1) << "|" << (int)immh;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "EĞER" || T.OPCODE == "eğer") {
            if (T.MODE == true) {
                Out << 0x10 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2);
            } else if (T.MODE == false) {
                Out << 0x11 << "|" << (int)reg(T.DATA1) << "|" << (int)immh;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "BÜYÜK" || T.OPCODE == "büyük") {
            if (T.MODE == true) {
                Out << 0x12 << "|" << (int)reg(T.DATA1);
            } else if (T.MODE == false) {
                Out << 0x13 << "|" << (int)imml;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "KÜÇÜK" || T.OPCODE == "küçük") {
            if (T.MODE == true) {
                Out << 0x14 << "|" << (int)reg(T.DATA1);
            } else if (T.MODE == false) {
                Out << 0x15 << "|" << (int)imml;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "DOĞRU" || T.OPCODE == "doğru") {
            if (T.MODE == true) {
                Out << 0x16 << "|" << (int)reg(T.DATA1);
            } else if (T.MODE == false) {
                Out << 0x17 << "|" << (int)imml;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "YANLIŞ" || T.OPCODE == "yanlış") {
            if (T.MODE == true) {
                Out << 0x18 << "|" << (int)reg(T.DATA1);
            } else if (T.MODE == false) {
                Out << 0x19 << "|" << (int)imml;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "ZIPLA" || T.OPCODE == "zıpla") {
            if (T.MODE == true) {
                Out << 0x1A << "|" << (int)reg(T.DATA1);
            } else if (T.MODE == false) {
                Out << 0x1B << "|" << (int)imml;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "RYAZ" || T.OPCODE == "ryaz") {
            Out << 0x1C << "|";
        }

        if (T.OPCODE == "ROKU" || T.OPCODE == "roku") {
            Out << 0x1D << "|";
        }

        if (T.OPCODE == "PYAZ" || T.OPCODE == "pyaz") {
            if (T.DATA1.substr(T.DATA1.size(), 1) == "r" || T.DATA1.substr(T.DATA1.size(), 1) == "R") {
                Out << 0x1E << "|" << (int)reg(T.DATA1);
            } else if (T.DATA1.substr(T.DATA1.size(), 1) != "r" || T.DATA1.substr(T.DATA1.size(), 1) != "R") {
                Out << 0x1F << "|" << (int)imml;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "POKU" || T.OPCODE == "poku") {
            if (T.DATA1.substr(T.DATA1.size() - 1, 1) == "r" || T.DATA1.substr(T.DATA1.size() - 1, 1) == "R") {
                Out << 0x20 << "|" << (int)reg(T.DATA1);
            } else if (T.DATA1.substr(T.DATA1.size() - 1, 1) != "r" || T.DATA1.substr(T.DATA1.size() - 1, 1) != "R") {
                Out << 0x21 << "|" << (int)imml;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "VE" || T.OPCODE == "ve") {
            if (T.MODE == true) {
                Out << 0x22 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2);
            } else if (T.MODE == false) {
                Out << 0x23 << "|" << (int)reg(T.DATA1) << "|" << (int)immh;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "VEYA" || T.OPCODE == "veya") {
            if (T.MODE == true) {
                Out << 0x24 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2);
            } else if (T.MODE == false) {
                Out << 0x25 << "|" << (int)reg(T.DATA1) << "|" << (int)immh;
            } else {
                continue;
            }
        }

        if (T.OPCODE == "DEĞİL" || T.OPCODE == "değil") {
            if (T.MODE == true) {
                Out << 0x26 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2);
            } else if (T.MODE == false) {
                Out << 0x27 << "|" << (int)reg(T.DATA1) << "|" << (int)immh;
            } else {
                continue;
            }
		}

        Out << endl;
    }
}
