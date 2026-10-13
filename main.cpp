#include <iostream>
#include <vector>
#include <string>
#include "LogRecord.h"
#include "PipelineStage.h"
#include "ConsoleSink.h"
#include "IpFilter.h"
#include "StatusCodeFilter.h"
#include "MethodFilter.h"
#include "ParserStage.h"

using namespace std;

void anaMenuGoster() {
    cout << "\n=========================================" << endl;
    cout << "     LOGFLOW PIPELINE - ANA MENU         " << endl;
    cout << "=========================================" << endl;
    cout << "1. Filtre Sec ve Uygula" << endl;
    cout << "2. Loglari Isle ve Baslat" << endl;
    cout << "3. Cikis" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Seciminiz (1-3): ";
}

int main() {
    // Aktif filtre tercihleri
    bool useMethod = false;
    bool useStatus = false;
    bool useIp = false;

    string targetMethod = "GET";
    int targetStatus = 200;
    string targetIp = "127.0.0.1";

    // Örnek log simülasyon verileri
    vector<string> sampleLogs = {
        "127.0.0.1 - - [10/Oct/2026:13:31:00 +0300] \"GET /index.html HTTP/1.1\" 200 1024",
        "192.168.1.50 - - [10/Oct/2026:13:32:15 +0300] \"POST /api/login HTTP/1.1\" 200 512",
        "127.0.0.1 - - [10/Oct/2026:13:33:40 +0300] \"GET /about.html HTTP/1.1\" 404 128",
        "BOZUK_VE_HATALI_LOG_SATIRI", // Hatalý satýr (ayrýþtýrýcý tarafýndan sayýlacak)
        "127.0.0.1 - - [10/Oct/2026:13:35:10 +0300] \"GET /contact.html HTTP/1.1\" 200 256"
    };

    int secim = 0;
    do {
        anaMenuGoster();
        cin >> secim;

        switch (secim) {
            case 1: // Filtre Seçim Listesi
            {
                int filtreSecim;
                cout << "\n=========================================" << endl;
                cout << "       MEVCUT FILTRE SECENEKLERI         " << endl;
                cout << "=========================================" << endl;
                cout << "1. HTTP Metot Filtresi (Orn: GET / POST)" << endl;
                cout << "2. HTTP Durum Kodu Filtresi (Orn: 200 / 404)" << endl;
                cout << "3. IP Adresi Filtresi (Orn: 127.0.0.1)" << endl;
                cout << "-----------------------------------------" << endl;
                cout << "Hangi filtreyi secmek istiyorsunuz? (1-3): ";
                cin >> filtreSecim;

                if (filtreSecim == 1) {
                    useMethod = true;
                    cout << "-> HTTP Metot Filtresi seçildi." << endl;
                    cout << "   Filtrelenecek Metotu Girin (Örn: GET): ";
                    cin >> targetMethod;
                    cout << "[BASARILI] Metot filtresi aktif edildi!" << endl;
                } 
                else if (filtreSecim == 2) {
                    useStatus = true;
                    cout << "-> HTTP Durum Kodu Filtresi seçildi." << endl;
                    cout << "   Filtrelenecek Durum Kodunu Girin (Örn: 200): ";
                    cin >> targetStatus;
                    cout << "[BASARILI] Durum kodu filtresi aktif edildi!" << endl;
                } 
                else if (filtreSecim == 3) {
                    useIp = true;
                    cout << "-> IP Adresi Filtresi seçildi." << endl;
                    cout << "   Filtrelenecek IP Adresini Girin (Örn: 127.0.0.1): ";
                    cin >> targetIp;
                    cout << "[BASARILI] IP filtresi aktif edildi!" << endl;
                } 
                else {
                    cout << "[HATA] Gecersiz filtre numarasi girdiniz!" << endl;
                }
                break;
            }
            case 2: // Loglarý Ýþle ve Boru Hattýný Çalýþtýr
            {
                cout << "\n=========================================" << endl;
                cout << "   LOGLAR ISLENIYOR (Secilen Filtreler)  " << endl;
                cout << "=========================================" << endl;
                
                // Boru hattý dinamik olarak kuruluyor (Sondan baþa)
                ConsoleSink consoleSink;
                PipelineStage* currentPipeline = &consoleSink;

                IpFilter* ipFilter = nullptr;
                StatusCodeFilter* statusFilter = nullptr;
                MethodFilter* methodFilter = nullptr;

                if (useIp) {
                    ipFilter = new IpFilter(targetIp, currentPipeline);
                    currentPipeline = ipFilter;
                    cout << "-> Aktif Filtre: IP = " << targetIp << endl;
                }
                if (useStatus) {
                    statusFilter = new StatusCodeFilter(targetStatus, currentPipeline);
                    currentPipeline = statusFilter;
                    cout << "-> Aktif Filtre: Status = " << targetStatus << endl;
                }
                if (useMethod) {
                    methodFilter = new MethodFilter(targetMethod, currentPipeline);
                    currentPipeline = methodFilter;
                    cout << "-> Aktif Filtre: Method = " << targetMethod << endl;
                }

                if (!useIp && !useStatus && !useMethod) {
                    cout << "-> [UYARI] Hicbir filtre secilmedi, loglar dogrudan yazdirilacak." << endl;
                }

                cout << "-----------------------------------------" << endl;

                // Ayrýþtýrýcý oluþturulur ve loglar iþleme sokulur
                ParserStage parser(currentPipeline);

                for (size_t i = 0; i < sampleLogs.size(); ++i) {
                    parser.parseAndProcess(sampleLogs[i]);
                }

                cout << "\n-----------------------------------------" << endl;
                cout << "ISLEM RAPORU:" << endl;
                cout << "Toplam Hatali/Atlanan Satir Sayisi: " << parser.getErrorCount() << endl;
                cout << "=========================================" << endl;

                // Dinamik bellek temizliði
                delete methodFilter;
                delete statusFilter;
                delete ipFilter;

                break;
            }
            case 3:
                cout << "Programdan cikiliyor. Iyi günler!" << endl;
                break;
            default:
                cout << "Gecersiz secim! Lutfen 1, 2 veya 3 girin." << endl;
        }

    } while (secim != 3);

    return 0;
}
