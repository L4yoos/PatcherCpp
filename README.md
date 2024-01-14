# Patcher C++ | L4yoos
Projekt Updater C++ to aplikacja umożliwiająca pobieranie plików z serwera, sprawdzanie ich integralności za pomocą sumy kontrolnej CRC-32 oraz zarządzanie aktualizacjami. Poniżej znajdziesz informacje dotyczące funkcji głównych, implementacji.

## Główne Funkcje
#### Pobieranie i Aktualizacja Plików
- Aplikacja umożliwia pobieranie plików z serwera za pomocą protokołu HTTP.
- Sprawdzanie integralności plików poprzez obliczanie sumy kontrolnej CRC-32.
- Automatyczna aktualizacja plików, jeżeli zostanie wykryta rozbieżność sumy kontrolnej lub rozmiaru pliku.

#### Obsługa Błędów i Logowanie
- Precyzyjne obsługiwanie błędów związanych z operacjami plikowymi i pobieraniem.
- Szczegółowe logowanie, które ułatwia śledzenie działań aplikacji.

## Implementacja
#### Wykorzystane Biblioteki
- Libcurl: Wykorzystane do obsługi operacji HTTP, umożliwiając pobieranie plików z serwera.
- Boost: Stosowane w celu obliczania sumy kontrolnej CRC-32 dla zapewnienia integralności plików.
- FileSystem (C++17): Używane do operacji na systemie plików, takich jak sprawdzanie istnienia plików i usuwanie ich.

#### Funkcje Pomocnicze
- WriteCallback: Funkcja callback używana przez Libcurl do zapisywania danych pobranych z serwera.
- fileExists: Sprawdza istnienie pliku na lokalnym systemie plików.
- calculateCRC: Oblicza sumę kontrolną CRC-32 dla danego pliku.
- getFileSize: Zwraca rozmiar pliku.
## Zawartość patchlist.txt

| Nazwa Pliku | CRC-32    | Rozmiar Pliku          |
| :-------- | :------- | :------------------------- |
| `plik1.exe` | `97823x19` | `258048` |
| `plik2.cfg` | `213c7d65` | `15` |
| `...` | `...` | `...` |


## Screenshots

![Screenshot](https://i.imgur.com/jk0rjVQ.png)



## Authors

- [@L4yoos](https://www.github.com/L4yoos)
