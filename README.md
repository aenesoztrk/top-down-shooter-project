# 2D Top-Down Shooter Oyunu

🔗 **GitHub Repository:** [https://github.com/aenesoztrk/top-down-shooter-project](https://github.com/aenesoztrk/top-down-shooter-project)

Bu proje, C++ ve [SFML 3.0](https://www.sfml-dev.org/) kullanılarak geliştirilmiş, yukarıdan bakış (top-down) açısına sahip 2 boyutlu bir nişancı (shooter) oyunudur. 

## Özellikler

- **Dalga (Wave) Sistemi:** Oyun ilerledikçe düşmanların sayısı ve hızı artar.
- **Dinamik Karakter Yönlendirmesi:** Karakter, fare imlecinin olduğu yöne dönerek ateş eder.
- **Güçlendirmeler (Power-ups):** Düşmanlardan düşen sağlık (HP) ve hız güçlendirmeleri.
- **Puanlama Sistemi:** Altın (Coin) toplayarak skoru artırma ve kalıcı en yüksek skor kaydı (highscore.txt).
- **Modern C++ Standartları:** OOP prensipleri ve C++17 kullanılarak yapılandırılmıştır.

## Gereksinimler

Projeyi derlemek ve çalıştırmak için aşağıdaki araçlara ihtiyacınız vardır:
- C++17 destekleyen bir derleyici (GCC, Clang vb.)
- [CMake](https://cmake.org/) (3.15 veya üzeri)
- [SFML 3.0](https://www.sfml-dev.org/) kütüphanesi

*(macOS üzerinde Homebrew ile kurulum: `brew install sfml`)*

## Derleme ve Çalıştırma

Terminal veya komut satırını açarak proje dizininde şu adımları izleyin:

1. Derleme klasörünü oluşturun ve CMake ile konfigüre edin:
   ```bash
   cmake -B build
   ```
2. Projeyi derleyin:
   ```bash
   cmake --build build
   ```
3. Oyunu çalıştırın:
   ```bash
   ./build/TopDownShooter
   ```

## Kontroller

- **Hareket:** W, A, S, D tuşları
- **Nişan Alma:** Fare (Mouse) imleci
- **Ateş Etme:** Fare Sol Tık

## Demo Videosu

Oyunun oynanışına ve çalışma mantığına ait demo videosunu izlemek için şu adresi ziyaret edebilirsiniz:  
🔗 **[programlama.okiyy.com](http://programlama.okiyy.com)**

## Geliştirici
**Ahmet Enes ÖZTÜRK** (Öğrenci No: 250229027)
