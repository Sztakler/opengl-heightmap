# HeightMap

Program, służący do wyświetlania powierzchni Ziemi w trybie 2D (mapa) i 3D (globus) z użyciem plików .hgt.

## Użycie

Do skompilowania programu wystarczy użyć `make`
`$ make`
a następnie, by uruchomić program należy użyć:
`./heightmap`

Program może zostać uruchomiony z maksymalnie 7 parametrami, podanymi w poniższej kolejności:

`./heighmap maps_catalog offset -lat x y -lon x y`

* map_catalog -- ścieżka do katalogu, zawierającego pliki lub podkatalogi z mapami .hgt. Program wczytuje jedynie pliki o rozszerzeniu .hgt. Katalog z mapami powinien znajdować się wewnątrz folderu z programem, np. można umieścić dane w folderze `maps`.
* offset -- w przypadku ładowania dużej liczby plików .hgt program może uruchamiać się bardzo długo. Żeby przyspieszyć ten proces zaimplementowałem mechanizm szybkiego ładowania. Wystarczy podać wartość parametru offset, np. 10, a program będzie wtedy wczytywał z pliku wysokości o współrzędnych, będących wielokrotnością offsetu.
* -lat x y / -lon x y -- podanie tych parametrów każde programowi wczytywać jedynie te pliki, które opisują współrzędne geograficzne o szerokości z przedziału x,y (włączając krańce). Pliki .hgt występują z nazwami [N/S]x[E/W]y, które oznaczają gdzie [N/S] to odpowiednio szerokość geograficzna na północ i południe od równika i [E\W] długość geograficzną na wschód i zachód od południka 0, jednak zawsze są one liczbą dodatnią. Program rozpoznaje parametry x i y jako liczby ze znakiem, tj. pliki wartości S i W są ujemne, a N i E dodatnie.

Z programu można wyjść klawiszem ESC.

### Przykład:

Chcemy załadować pliki z folderu `maps` o szerokościach S20-N20 i długościach W50-E50 i wczytywać co 10 wysokość:
`./heightmap maps 10 -lat -20 20 -lon -50 50`.

## Sterowanie

Program obsługuje 2 tryby wyświetlania mapy:
* mapa 2D,
* globus 3D (domyślny).

Możliwe jest zmienianie trybu wyświetlania w trakcie działania programu poprzez zmianę shadera (klawisz M) i/lub zmianę kamery (klawisz TAB).

Program obsługuje zmianę poziomu detali. Poziom detali można zmienić, używając klawiszy od 1 (najwyższe detale) do 7 (najniższe detale). Klawisz 0 aktywuje tryb automatycznego dostosowania poziomu detali, w zależności od aktualnej liczby FPS (utrzymuje je powyżej 10).

### Tryb 2D (wolna kamera)

* Poruszanie się -- klawisze WSAD.
* Zwiększenie wysokości kamery -- klawisz SPACJA.
* Zmniejszenie wysokości kamery -- klawisz SHIFT.
* Zoom -- kółko myszy.

### Tryb 3D (kamera arcball)

* Obracanie globu -- wciśnięty lewy przycisk myszy i poruszanie myszą (ewentualnie klawisze WSAD lub strzałki).
* Obracanie kamery -- wciśnięty środkowy przycisk myszy i poruszanie myszą.
* Zwiększenie wysokości kamery -- klawisz SPACJA.
* Zmniejszenie wysokości kamery -- klawisz SHIFT.
* Zoom -- kółko myszy.

Prędkość kamery jest uzależniona od wysokości nad powierzchnią mapy lub globusu. Im kamera jest niżej, tym wolniej się porusza, umożliwiając precyzyjniejsze ruchy.
