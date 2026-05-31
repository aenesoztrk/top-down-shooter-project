# Top-Down Shooter Proje Dokümantasyonu

**Geliştirici:** Ahmet Enes ÖZTÜRK  
**Öğrenci Numarası:** 250229027  
**Proje Adı:** 2D Top-Down Shooter (Yukarıdan Bakışlı Nişancı Oyunu)

---

## 1. Giriş

Bu proje, oyuncunun yukarıdan bakış (top-down) açısıyla bir karakteri kontrol ederek dalgalar (wave) halinde üzerine gelen düşmanlara karşı hayatta kalmaya çalıştığı, 2 boyutlu (2D) bir aksiyon oyunudur. Projenin temel amacı, C++ programlama dili ve nesne yönelimli programlama (OOP) prensiplerini kullanarak, modern grafik kütüphaneleri yardımıyla baştan sona işlevsel bir oyun döngüsü kurabilmektir. Oyunda düşmanları vurma, para (coin) toplama, can (HP) veya hız güçlendirmeleri (power-up) alma ve ulaşılan en yüksek skoru kalıcı olarak saklama gibi mekanikler bulunmaktadır.

## 2. Gelişme

Bu bölümde projede kullanılan teknolojiler, oyunun arka plandaki mimarisi, karşılaşılan problemler ve bunlara üretilen teknik çözümler açıklanmıştır.

### 2.1. Kullanılan Teknolojiler
- **Programlama Dili:** C++17 standartları kullanıldı.
- **Grafik ve Pencere Kütüphanesi:** SFML (Simple and Fast Multimedia Library) 3.0 kullanılarak pencere yönetimi, olay (event) yakalama, şekil ve sprite çizimi sağlandı.
- **Derleme Sistemi:** CMake kullanılarak platform bağımsız bir yapı oluşturuldu ve kütüphane bağlantıları (linking) ayarlandı.
- **Editör/Ortam:** VS Code ve MacOS ortamında geliştirildi.

### 2.2. Oyun Mimarisi ve Tasarımı
Proje, her biri kendi işlevinden sorumlu olan çeşitli sınıflara (class) bölünmüştür:
- **`Player` Sınıfı:** Oyuncunun hareketini (WASD), sağlığının (HP) kontrolünü, silah bekleme süresini (cooldown) ve fare imlecine göre açısını hesaplayan sınıftır.
- **`Enemy` Sınıfı:** Oyun karakterine doğru sürekli yaklaşan ve dalga (wave) seviyesi arttıkça hızlanan düşmanları barındırır.
- **`Bullet` Sınıfı:** Karakterin ateşleme açısından yola çıkarak doğrusal bir vektörde hareket eden mermileri yönetir.
- **`PowerUp` & `Coin` Sınıfları:** Düşman öldüğünde rastgele oluşan faydalı eşyaları (can, hız, skor) temsil eder.

Bu nesneler bellek sızıntılarını önlemek ve pratik olmak amacıyla `std::vector` yapıları içinde dinamik bir şekilde tutulmuştur.

### 2.3. Karşılaşılan Sorunlar ve Çözümleri
Projenin geliştirilme sürecinde çeşitli algoritmik ve donanımsal mantık problemleriyle karşılaşılmış ve çözülmüştür:

1. **SFML Kurulumu ve IntelliSense (Clangd) Hataları:**
   - *Sorun:* Editör, SFML kütüphanesini tanımadığı için başlık (header) dosyalarında hatalar alındı.
   - *Çözüm:* `CMakeLists.txt` içerisine `CMAKE_EXPORT_COMPILE_COMMANDS ON` eklenip, `compile_commands.json` oluşturulduktan sonra VS Code'un include yollarını (opt/homebrew/include) doğru okuması sağlandı.

2. **Karakterin İmlece Doğru Dönmesi ve Ters (Baş Aşağı) Durma Problemi:**
   - *Sorun:* Karakter, farenin bulunduğu yöne bakarken (`std::atan2` kullanarak) 90 ve 270 dereceleri geçince görselin baş aşağı (ters) dönmesi kötü bir görüntü yaratıyordu.
   - *Çözüm:* Farenin X koordinatı merkeze alınarak ekranın sağına ve soluna bakış durumu ikiye ayrıldı. Sol tarafa bakıldığında sol sprite (`player_left.png`), sağa bakıldığında sağ sprite (`player_right.png`) kullanıldı ve ölçeklendirmeler simetrik olarak ayarlandı. Aynı mantık düşmanların (enemy) ekranın sağından ve solundan gelmesine göre de uyarlandı.

3. **Veri Kalıcılığı (Kayıt Sistemi):**
   - *Sorun:* Oyun kapatıldığında oyuncunun kazandığı en yüksek skor/coin siliniyordu.
   - *Çözüm:* Standart C++ kütüphaneleri `std::ifstream` ve `std::ofstream` kullanılarak basit bir kayıt sistemi yazıldı. Oyun bittiğinde eğer yeni skor eskisinden büyükse `highscore.txt` dosyasına kaydedildi ve açılışta tekrar okundu.

### 2.4. Nasıl Oynanır?
Oyun açıldığında karşınıza çıkan menüden **OYUNA BAŞLA** butonuna basarak giriş yapabilirsiniz.
- **Yönlendirme:** `W`, `A`, `S`, `D` tuşları ile karakterinizi çimenli alanda hareket ettirin.
- **Ateş Etme:** Fare (mouse) imlecini düşmanlara doğrultarak hedefinizi alın ve sol tık basılı tutarak mermi atın.
- **Toplama:** Düşmanlar öldüğünde yere sarı altınlar (Coin) ve güçlendirmeler bırakır.
  - *Yeşil Kutu:* Canınızı (HP) doldurur.
  - *Mavi Kutu:* Karakterinizin hareket hızını geçici olarak artırır.
- **Amacınız:** Dalgalar (Wave) geçtikçe daha da hızlanan ve çoğalan düşmanlardan kaçarak, ölmeden en çok coini toplamaktır!

### 2.5. Demo Videosu
Oyunun oynanışına ve proje geliştirme aşamalarına dair hazırlanan demo videosuna aşağıdaki bağlantıdan ulaşabilirsiniz:
🔗 **[programlama.okiyy.com](http://programlama.okiyy.com)**

## 3. Sonuç

"2D Top-Down Shooter" projesi, oyun döngüsünün, çarpışma kontrollerinin (collision detection), olay dinleyicilerin (event listeners) ve temel vektör matematiğinin nasıl bir arada çalıştığını kavramak adına mükemmel bir temel oluşturdu. Yazılan C++ kodu tamamen nesne yönelimli olarak geliştirildiğinden, gelecekte yeni silah türleri, yeni düşman tipleri veya menü tasarımları eklemek oldukça kolay bir altyapıya sahiptir. 

Hazırlayan: Ahmet Enes ÖZTÜRK (250229027)
