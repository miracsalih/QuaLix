// This file is compiled using a Linux system. It will not work on Windows. However, it is theoretically compatible with macOS, but this is not guaranteed.
#define Q "\n"
#include <complex>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

struct INFO {
    uint8_t dil = -1;
    uint32_t counter = 0;
    uint32_t uinter = 0;
    uint8_t bit = 0;
    uint8_t mod = 0;
    string OUT = "";
};

INFO info;

struct T {
    string OPCODE;        // KAPI, RAM, PORTA vs.
    string MODE;          // 0, 1, 2, 3
    string DATA1;         // 123, 0xAA (hedef)
    string DATA2;         // 456, 0xBB (kaynak)
};

T T;

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
    } else if (reg_id == "R0" || reg_id == "r0") {
        return 6;
    } else if (reg_id == "R1" || reg_id == "r1") {
        return 7;
    } else if (reg_id == "R2" || reg_id == "r2") {
        return 8;
    } else if (reg_id == "R3" || reg_id == "r3") {
        return 9;
    } else if (reg_id == "R4" || reg_id == "r4") {
        return 10;
    } else if (reg_id == "R5" || reg_id == "r5") {
        return 11;
    } else if (reg_id == "R6" || reg_id == "r6") {
        return 12;
    } else if (reg_id == "R7" || reg_id == "r7") {
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

    while (getline(File, KOD)) {
        T.OPCODE = "";
        T.MODE = "";
        T.DATA1 = "";
        T.DATA2 = "";
        info.uinter++;
        i = 0;
        info.counter = 0;

        while (true) {
            c = KOD.substr(i, 1);

            if (c == ";") {
                break;
            }

            d = KOD.substr(i + 1, 1);

            if (c == " ") {
                if (d != " " && d != ",") {
                    info.counter++;
                }
            } else if (c != " ") {
                if (info.counter == 0) T.OPCODE += c;
                if (info.counter == 1) T.MODE += c;
                if (info.counter == 2) T.DATA1 += c;
                if (info.counter == 3) T.DATA2 += c;
            }
            i++;
        }

        if (T.OPCODE == "TAŞI" || T.OPCODE == "taşı") {
            if (T.MODE == "REG" || T.MODE == "reg") {
                Out << 0x2 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|";
            } else if (T.MODE == "IMM" || T.MODE == "imm") {
                uint32_t a = stoi(T.DATA2, nullptr, 10);
                Out << 0x3 << "|" << (int)reg(T.DATA1) << "|" << (int)a << "|";
            } else {
                continue;
            }
        }

        if (T.OPCODE == "EKLE" || T.OPCODE == "ekle") {
            if (T.MODE == "REG" || T.MODE == "reg") {
                Out << 0x4 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2);
            } else if (T.MODE == "IMM" || T.MODE == "imm") {
                uint32_t a = stoi(T.DATA2, nullptr, 10);
                Out << 0x5 << "|" << (int)reg(T.DATA1) << "|" << (int)a << "|";
            } else {
                continue;
            }
        }

        if (T.OPCODE == "ÇIKAR" || T.OPCODE == "çıkar") {
            if (T.MODE == "REG" || T.MODE == "reg") {
                Out << 0x6 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|";
            } else if (T.MODE == "IMM" || T.MODE == "imm") {
                uint32_t a = stoi(T.DATA2, nullptr, 10);
                Out << 0x7 << "|" << (int)reg(T.DATA1) << "|" << (int)a << "|";
            } else {
                continue;
            }
        }

        if (T.OPCODE == "--") {
            Out << 0x8 << "|" << (int)reg(T.MODE) << "|";
        }

        if (T.OPCODE == "++") {
            Out << 0xA << "|" << (int)reg(T.MODE) << "|";
        }

        if (T.OPCODE == "BÖL" || T.OPCODE == "böl") {
            if (T.MODE == "REG" || T.MODE == "reg") {
                Out << 0xC << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|";
            } else if (T.MODE == "IMM" || T.MODE == "imm") {
                uint32_t a = stoi(T.DATA2, nullptr, 10);
                Out << 0xD << "|" << (int)reg(T.DATA1) << "|" << (int)a << "|";
            } else {
                continue;
            }
        }

        if (T.OPCODE == "ÇARP" || T.OPCODE == "çarp") {
            if (T.MODE == "REG" || T.MODE == "reg") {
                Out << 0xE << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|";
            } else if (T.MODE == "IMM" || T.MODE == "imm") {
                uint32_t a = stoi(T.DATA2, nullptr, 10);
                Out << 0xF << "|" << (int)reg(T.DATA1) << "|" << (int)a << "|";
            } else {
                continue;
            }
        }

        if (T.OPCODE == "EĞER" || T.OPCODE == "eğer") {
            if (T.MODE == "REG" || T.MODE == "reg") {
                Out << 0x10 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|";
            } else if (T.MODE == "IMM" || T.MODE == "imm") {
                uint32_t a = stoi(T.DATA2, nullptr, 10);
                Out << 0x11 << "|" << (int)reg(T.DATA1) << "|" << (int)a << "|";
            } else {
                continue;
            }
        }

        if (T.OPCODE == "BÜYÜK" || T.OPCODE == "büyük") {
            Out << 0x12 << "|" << T.MODE << "|";
        }

        if (T.OPCODE == "DOĞRU" || T.OPCODE == "doğru") {
            Out << 0x14 << "|" << T.MODE << "|";
        }

        if (T.OPCODE == "KÜÇÜK" || T.OPCODE == "küçük") {
            Out << 0x16 << "|" << T.MODE << "|";
        }

        if (T.OPCODE == "YANLIŞ" || T.OPCODE == "yanlış") {
            Out << 0x18 << "|" << T.MODE << "|";
        }

        if (T.OPCODE == "ZIPLA" || T.OPCODE == "zıpla") {
            Out << 0x1A << "|" << T.MODE << "|";
        }

        if (T.OPCODE == "REB" || T.OPCODE == "reb") {
            if (T.MODE == "YAZ" || T.MODE == "yaz") {
                Out << 0x1C << "|0|";
            } else if (T.MODE == "OKU" || T.MODE == "oku") {
                Out << 0x1D << "|0|";
            } else {
                continue;
            }
        }

        if (T.OPCODE == "PORT" || T.OPCODE == "port") {
            if (T.MODE == "YAZ" || T.MODE == "yaz") {
                Out << 0x1E << "|" << stoi(T.DATA1, nullptr, 10) << "|";
            } else if (T.MODE == "OKU" || T.MODE == "oku") {
                Out << 0x1F << "|" << stoi(T.DATA1, nullptr, 10) << "|";
            } else {
                continue;
            }
        }

        if (T.OPCODE == "VE" || T.OPCODE == "ve") {
            if (T.MODE == "REG" || T.MODE == "reg") {
                Out << 0x20 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|";
            } else if (T.MODE == "IMM" || T.MODE == "imm") {
                uint32_t a = stoi(T.DATA2, nullptr, 10);
                Out << 0x20 << "|" << (int)reg(T.DATA1) << "|" << (int)a << "|";
            } else {
                continue;
            }
        }

        if (T.OPCODE == "VEYA" || T.OPCODE == "veya") {
            if (T.MODE == "REG" || T.MODE == "reg") {
                Out << 0x22 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|";
            } else if (T.MODE == "IMM" || T.MODE == "imm") {
                uint32_t a = stoi(T.DATA2, nullptr, 10);
                Out << 0x23 << "|" << (int)reg(T.DATA1) << "|" << (int)a << "|";
            } else {
                continue;
            }
        }

        if (T.OPCODE == "DEĞİL" || T.OPCODE == "değil") {
            if (T.MODE == "REG" || T.MODE == "reg") {
                Out << 0x24 << "|" << (int)reg(T.DATA1) << "|" << (int)reg(T.DATA2) << "|";
            } else if (T.MODE == "IMM" || T.MODE == "imm") {
                uint32_t a = stoi(T.DATA2, nullptr, 10);
                Out << 0x25 << "|" << (int)reg(T.DATA1) << "|" << (int)a << "|";
            } else {
                continue;
            }
        }

        /*if (T.OPCODE == "/ygib" || T.OPCODE == "/ygib") {             // DENEYSEL!!!
            uint32_t a = stoi(T.DATA2, nullptr, 10);
            Out << 0x3 << "|" << 6 << "|" << (int)a << "|";
        }*/

        Out << endl;
    }
}
