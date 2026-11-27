# Semestrální práce B6B36PCC: Hra 2048

* **Předmět:** B6B36PCC – Programování v C/C++ (ČVUT FEL)
* **Autor:** Vasyl Klius
* **Název projektu:** 2048 Console Game
* **Verze / Commit hash:** `<DOPLŇTE_HASH_POSLEDNÍHO_COMMITU>`
* **Prezentace:** `https://gitlab.fel.cvut.cz/kliusvas/2048/docs/Prezentace2048VasylKlius.pdf`
* **Odkazy na testovací videa (GitLab FEL):**
  * **Video 1 (Postup překladu):** `https://gitlab.fel.cvut.cz/kliusvas/2048/docs/video_build.mp4`
  * **Video 2 (Průběh hry a testování se zvukovým komentářem):** `https://gitlab.fel.cvut.cz/kliusvas/2048/docs/video_game.mp4`

---

## 1. Popis hry a herních strategií

### 1.1 Pravidla hry
**2048** je populární logická posuvná hra, kterou v roce 2014 původně vytvořil Gabriele Cirulli. Hra se odehrává na mřížce o rozměrech $N \times N$ (výchozí velikost je $4 \times 4$).

1. **Začátek hry:** Na hrací ploše se na náhodných volných pozicích vygenerují dvě počáteční dlaždice s hodnotou 2 (s 90% pravděpodobností) nebo 4 (s 10% pravděpodobností).
2. **Pohyb:** Hráč může v každém tahu posunout všechny dlaždice jedním ze čtyř směrů: **Nahoru (Up)**, **Dolů (Down)**, **Doleva (Left)** nebo **Doprava (Right)**.
3. **Slučování:** Pokud při posunu do sebe narazí dvě dlaždice se stejným číslem, sloučí se v jednu dlaždici s dvojnásobnou hodnotou ($2 + 2 = 4$, $4 + 4 = 8$, ..., $1024 + 1024 = 2048$).
   * **Pravidlo jednoho sloučení:** Během jednoho tahu se každá dlaždice může sloučit maximálně jednou (např. řada `[2, 2, 2, 2]` se při tahu doleva sloučí na `[4, 4, 0, 0]`, nikoli na `[8, 0, 0, 0]`).
4. **Skóre:** Při každém sloučení dlaždic se k celkovému skóre hráče přičte hodnota nově vzniklé dlaždice.
5. **Generování nové dlaždice:** Po každém platném tahu, který změnil stav desky, se na náhodném volném místě objeví nová dlaždice (hodnota 2 nebo 4).
6. **Výhra:** Hráč vyhrává, jakmile se na hrací desce objeví dlaždice s cílovou hodnotou (standardně **2048**, lze upravit přepínačem `-t`). Po dosažení výhry může hráč pokračovat dál ve hře.
7. **Prohra (Game Over):** Hra končí, pokud je celé hrací pole zaplněno a není možný žádný platný tah (žádné dvě sousední dlaždice nemají stejnou hodnotu).

### 1.2 Herní strategie
Pro úspěšné dosažení hodnoty 2048 se doporučuje dodržovat následující strategie:
* **Rohová strategie (Corner Strategy):** Udržovat dlaždici s nejvyšší hodnotou trvale v jednom z rohů hrací plochy (nejčastěji v pravém dolním nebo levém dolním rohu).
* **Monotónnost:** Řadit dlaždice v řádcích a sloupcích sestupně podle jejich hodnot (např. v dolním řádku zprava doleva: $1024 \to 512 \to 256 \to 128$).
* **Zákaz opačného pohybu:** Pokud je nejvyšší číslo v dolním rohu, vyhýbat se tahu nahoru (Up), aby se rohová pozice neuvolnila a nezaplnila novou dlaždicí s hodnotou 2.
* **Využití funkce Undo:** V případě nechtěného přehmatu nebo nepříznivého tahu může hráč využít klávesu `U` pro návrat o jeden krok zpět.

---

## 2. Popis postupu řešení práce a struktura tříd

Aplikace je navržena podle principů objektově orientovaného programování (OOP) v souladu se standardem **C++20**. Kód je rozdělen do samostatných modulů s jasně definovanými zodpovědnostmi.

### 2.1 Přehled tříd

```
┌────────────────────────────────────────────────────────┐
│                        main()                          │
│        (zpracování argumentů a spuštění hry)           │
└──────────────────────────┬─────────────────────────────┘
                           │
             ┌─────────────┴─────────────┐
             ▼                           ▼
      ┌──────────────┐            ┌──────────────┐
      │  ArgParser   │            │     Game     │
      └──────────────┘            └──────┬───────┘
                                         │
                         ┌───────────────┴───────────────┐
                         ▼                               ▼
                  ┌──────────────┐                ┌──────────────┐
                  │    Board     │◄───────────────│ BoardPrinter │
                  └──────┬───────┘                └──────────────┘
                         │
                         ▼
                  ┌──────────────┐
                  │    Utils     │
                  └──────────────┘
```

### 2.2 Popis jednotlivých tříd a jejich metod

#### 1. Třída `Board` (`Board.h`, `Board.cpp`)
Reprezentuje herní stav a logiku posunů.
* **Členské proměnné:**
  * `width`, `height`: Rozměry hrací desky (výchozí $4 \times 4$).
  * `target`: Cílová hodnota pro výhru (výchozí 2048).
  * `board`: 2D vektor `std::vector<std::vector<int>>` reprezentující mřížku.
  * `score`: Aktuální dosažené skóre.
  * `is_game_over`: Příznak ukončení hry.
  * `last_spawned_tile`: Souřadnice naposledy vygenerované dlaždice.
  * `prev_board`, `prev_score`, `has_undo_state`: Záloha stavu pro funkci Undo.
* **Klíčové metody:**
  * `Board(int width, int height, int target)`: Konstruktor inicializující rozměry a nulové pole.
  * `void generate_board()`: Vygeneruje počáteční 2 náhodné dlaždice.
  * `void reset()`: Resetuje stav hrací plochy, skóre a vygeneruje nové pole.
  * `bool move_up()`, `move_down()`, `move_left()`, `move_right()`: Provedou posun a sloučení ve vybraném směru. Vrátí `true`, pokud došlo ke změně desky.
  * `bool move_tile_*(...)`: Pomocné privátní metody zajišťující posun a sloučení konkrétní dlaždice s ochranou proti vícenásobnému sloučení (`last_merged`).
  * `void create_tile()`: Najde volné pozice a na jednu z nich náhodně umístí hodnotu 2 nebo 4.
  * `bool check_game_over()`: Zkontroluje, zda existují prázdná pole nebo možnost sloučení sousedních dlaždic.
  * `bool has_winner() const`: Vrátí `true`, pokud kterákoli dlaždice na ploše dosáhla hodnoty $\ge target$.
  * `bool undo()`: Obnoví předchozí stav desky a skóre (vrátí tah zpět).
  * `bool can_undo() const`: Informuje, zda je dostupný stav pro vrácení.
  * `get_tile(x, y)`, `set_tile(...)`, `get_score()`, `get_width()`, `get_height()`, `get_target()`: Přístupové metody (gettery a settery).

#### 2. Třída `BoardPrinter` (`BoardPrinter.h`, `BoardPrinter.cpp`)
Zabezpečuje formátovaný ASCII výstup hrací desky do libovolného `std::ostream`.
* **Klíčové metody:**
  * `BoardPrinter(Board &board)`: Konstruktor vázaný na instanci desky.
  * `int computeCellNumWidth() const`: Dynamicky spočítá potřebnou šířku buňky podle nejvyššího čísla na desce (minimálně 4 znaky), čímž zaručuje dokonale rovné svislé ohraničení bez deformací.
  * `void printBoard(std::ostream &out)`: Vykreslí celou desku s ohraničením `|`, `-`, správným zarovnáním a ANSI barvami.
  * `static const char* getTileColor(int val, bool is_new)`: Vrátí ANSI escape sekvenci barvy textu podle hodnoty dlaždice (zelená pro novou dlaždici, azurová pro 4, modrá pro 8, žlutá pro 16, červená pro 64, zlatá pro 2048 atd.).
  * `void printValueWithWhitespaces(...)`: Zarovná číslo v buňce doprava s přesným doplněním mezer.

#### 3. Třída `Game` (`Game.h`, `Game.cpp`)
Řídí běh aplikace a interakci s uživatelem.
* **Klíčové metody:**
  * `Game(int width, int height, int target)`: Inicializuje hru a tiskárnu.
  * `void run()`: Hlavní herní smyčka (Game Loop). V cyklu vykresluje hrací plochu a čeká na vstup uživatele.
  * `void render()`: Vymaže terminál (`\033[2J\033[H`), vykreslí záhlaví, aktuální skóre, počet tahů, desku a stav hry (Win / Game Over).
  * `void process_input()`: Přečte stisknutou klávesu ze standardního vstupu a provede odpovídající akci (pohyb, Undo, Restart, Quit).
  * `void set_raw_mode(bool enable)`: Pomocí rozhraní POSIX `termios` přepne terminál do nekanonického režimu bez vyčkávání na klávesu Enter a bez echa, po skončení hry obnoví původní nastavení.

#### 4. Třída `ArgParser` (`ArgParser.h`, `ArgParser.cpp`)
Zpracovává a validuje argumenty příkazové řádky.
* **Klíčové metody:**
  * `ArgParser(int argc, char *argv[])`: Zpracuje přepínače `-h/--help`, `-s/--size`, `-t/--target`, `--seed`.
  * `bool is_help_requested() const`: Test požadavku na nápovědu.
  * `bool has_error() const`: Indikuje syntaktickou nebo sémantickou chybu v argumentech.
  * `std::string get_error_message() const`: Popis vzniklé chyby.
  * `void print_help() const`: Vytiskne nápovědu s popisem všech voleb a ovládání.

#### 5. Třída `Utils` (`Utils.h`, `Utils.cpp`)
Poskytuje generátor pseudonáhodných čísel na bázi moderního algoritmu Mersenne Twister (`std::mt19937`).
* **Klíčové metody:**
  * `static int randint(int from, int to)`: Vrátí náhodné celé číslo v uzavřeném intervalu $\langle from, to \rangle$.
  * `static void set_seed(unsigned int seed)`: Umožňuje inicializovat generátor pevným seedem pro plně reprodukovatelné testování.

---

## 3. Postup kompilace programu a jeho spuštění

### 3.1 Požadavky na systém
* Překladač C++ podporující standard **C++20** (např. GCC $\ge 11$, Clang $\ge 13$, AppleClang $\ge 14$).
* Sestavovací systém **CMake** verze $\ge 3.20$.
* Operační systém: Linux, macOS nebo Windows se subsystémem WSL (doporučeno prostředí s podporou POSIX terminálu).

### 3.2 Postup překladu
V kořenovém adresáři projektu spusťte příkazy:
```bash
# 1. Konfigurace projektu přes CMake
cmake -B build

# 2. Překlad hlavní aplikace i testů
cmake --build build
```
Po úspěšném překladu vzniknou ve složce `build/` dva spustitelné soubory:
* `2048` – hlavní herní program.
* `unit_tests` – sada automatických jednotkových testů.

### 3.3 Spuštění hry
Základní spuštění s výchozími parametry ($4 \times 4$, cíl 2048):
```bash
./build/2048
```
Zobrazení nápovědy k programu:
```bash
./build/2048 --help
```

---

## 4. Popis přepínačů (argumentů příkazové řádky)

Program podporuje konfiguraci herních parametrů přímo z příkazové řádky:

| Přepínač | Dlouhý tvar | Parametr | Popis | Výchozí hodnota |
|---|---|---|---|---|
| `-h` | `--help` | – | Zobrazí nápovědu k programu a ukončí aplikaci | – |
| `-s` | `--size` | `<3..8>` | Nastaví rozměr hrací desky ($N \times N$) | `4` |
| `-t` | `--target` | `<N>` | Cílová hodnota dlaždice pro výhru (mocnina 2, $\ge 8$) | `2048` |
| – | `--seed` | `<číslo>` | Pevný seed pro generátor náhodných čísel (deterministický běh) | náhodný |

### Příklady spuštění:
```bash
# Rychlý test výhry na malé desce 3x3 s cílem 8 (výhra možná během 3 tahů)
./build/2048 -s 3 -t 8 --seed 100

# Hra na velké desce 5x5 s vyšším cílem 4096
./build/2048 --size 5 --target 4096

# Deterministická hra pro ověření stejného rozestavení dlaždic
./build/2048 --seed 42
```

### Ošetření chyb v argumentech:
Program provádí důkladnou validaci vstupních parametrů. V případě chyby vypíše srozumitelnou zprávu a skončí s návratovým kódem `1`:
* `./build/2048 -t 15` $\to$ `Error: Target must be a power of 2 (at least 8).`
* `./build/2048 -s 2` $\to$ `Error: Size must be between 3 and 8.`
* `./build/2048 --unknown` $\to$ `Error: Unrecognized option '--unknown'`

---

## 5. Popis ovládání aplikace

Komunikace s programem probíhá v textovém režimu terminálu. Vstup je zpracováván v reálném čase bez nutnosti potvrzování klávesou Enter.

### 5.1 Klávesové zkratky

| Klávesa | Akce |
|---|---|
| **`W`** nebo **Šipka Nahoru ($\uparrow$)** | Posun dlaždic nahoru |
| **`S`** nebo **Šipka Dolů ($\downarrow$)** | Posun dlaždic dolů |
| **`A`** nebo **Šipka Doleva ($\leftarrow$)** | Posun dlaždic doleva |
| **`D`** nebo **Šipka Doprava ($\rightarrow$)** | Posun dlaždic doprava |
| **`U`** | **Undo** – vrácení posledního tahu zpět |
| **`R`** | **Restart** – restartování hry a vygenerování nové desky |
| **`Q`** nebo **`Esc`** | **Quit** – korektní ukončení hry |

### 5.2 Vzhled uživatelského rozhraní

```text
========================================
               GAME 2048                
========================================
 Score:    128 | Moves:   24
----------------------------------------
-----------------------------
|      |    2 |    4 |      |
-----------------------------
|    8 |   16 |   32 |    2 |
-----------------------------
|      |    4 |   64 |  128 |
-----------------------------
|    2 |      |      |    4 |
-----------------------------
Legend:   *Green* = newly spawned tile
Controls: [W/A/S/D] or Arrows: Move | [U]: Undo | [R]: Restart | [Q]: Quit
```

* **Zvýraznění dlaždic:**
  * Naposledy vygenerovaná nová dlaždice je zvýrazněna **zeleným písmem** (`\033[1;92m`).
  * Každá hodnota dlaždice má své specifické ANSI zbarvení (2 bílá, 4 azurová, 8 modrá, 16 žlutá, 32 oranžová, 64 červená, 2048 zlatá).

---

## 6. Testování programu

Testování byla věnována mimořádná pozornost. Projekt obsahuje jak automatizované jednotkové testy (Unit Tests), tak vzorové testovací vstupy simulující reálné scénáře.

### 6.1 Automatické jednotkové testy (`unit_tests.cpp`)
V adresáři `tests/unit_tests.cpp` je implementováno 9 testovacích scénářů pokrývajících všechny kritické komponenty:

1. **`test_board_init`**: Ověření inicializace hrací plochy, rozměrů, skóre a vygenerování právě 2 počátečních dlaždic.
2. **`test_tile_movement`**: Test posunu dlaždic do volných buněk bez sloučení.
3. **`test_tile_merging`**: Test správného sloučení dvou stejných dlaždic ($2+2=4$) a přičtení správné hodnoty ke skóre.
4. **`test_no_double_merge`**: Ověření pravidla zákazu vícenásobného sloučení v jediném tahu ($[2, 2, 2, 2] \to [4, 4, 0, 0]$).
5. **`test_win_condition`**: Detekce výhry po sloučení dlaždic na cílovou hodnotu (`has_winner() == true`).
6. **`test_game_over`**: Detekce konce hry při plně obsazené desce bez možnosti dalších tahů (`get_is_game_over() == true`).
7. **`test_undo_functionality`**: Ověření návratu tahu – stav desky i skóre se vrátí na původní hodnotu, `can_undo()` se korektně resetuje.
8. **`test_argparser_valid`**: Ověření správného naparsování platných argumentů (`-s`, `-t`, `--seed`).
9. **`test_argparser_invalid`**: Ověření odmítnutí neplatných hodnot (neplatná velikost, cíl není mocnina 2, neznámý přepínač).

#### Spuštění jednotkových testů:
```bash
./build/unit_tests
# nebo přes CTest:
ctest --test-dir build --output-on-failure
```
**Výsledek:** `9 / 9 tests passed (100% success)`.

### 6.2 Vzorová vstupní data (`tests/inputs/`)
Pro testování chování celého programu v reálném běhu (přesměrováním vstupu) jsou připraveny testovací soubory:
* **`test_win.in`** (`wddq`): Test dosažení výhry během 3 tahů (`./build/2048 -s 3 -t 8 --seed 100 < tests/inputs/test_win.in`).
* **`test_undo.in`** (`ssuuq`): Test série posunů s následným dvojím vrácením tahu.
* **`test_restart.in`** (`wwwrq`): Test resetování hry klávesou `R`.
* **`test_invalid_keys.in`** (`xyz123wq`): Hraniční scénář – odeslání neočekávaných znaků; aplikace nehavaruje, ignoruje je a pokračuje.
* **`test_moves.in`** (`wasdwasdq`): Test kompletní sekvence tahů do všech směrů.

Všechny testy lze spustit hromadně pomocí přiloženého skriptu:
```bash
./tests/run_tests.sh
```

### 6.3 Kontrola správy paměti (AddressSanitizer)
Kód byl zkontrolován pomocí nástrojů `AddressSanitizer` (ASan) a `UndefinedBehaviorSanitizer` (UBSan) s volbami `-fsanitize=address,undefined -Wall -Wextra`:
* **Výsledek:** **0 chyb, 0 varování, 0 úniků paměti (memory leaks)**.

### 6.4 Časový harmonogram testovacího videa (Video 2)
Testovací video demonstruje funkčnost aplikace, mezní stavy a správné reakce na vstupy.

| Čas v nahrávce | Popis testovacího scénáře |
|---|---|
| **00:00 – 00:30** | Představení autora (ústně i písemně), název projektu a cíl práce. |
| **00:30 – 01:15** | Nápověda programu (`./2048 --help`) a demonstrace chybových hlášení při neplatných argumentech. |
| **01:15 – 02:45** | Běžný průběh hry: posuny ve všech 4 směrech, slučování dlaždic, nárůst skóre, barevné odlišení. |
| **02:45 – 03:45** | Demonstrace funkce **Undo** (vrácení nepříznivého tahu zpět, korekce skóre). |
| **03:45 – 04:45** | Hraniční situace: stisk nepovolených kláves (čísla, náhodná písmena), hra zůstává stabilní. |
| **04:45 – 06:15** | Scénář výhry: demonstrace dosažení cíle (`-t 8`) a zobrazení vítězné zprávy `YOU WIN!`. |
| **06:15 – 07:45** | Scénář prohry: zaplnění hrací plochy, zablokování dalších tahů a hlášení `GAME OVER!`. |
| **07:45 – 08:30** | Restart hry klávesou `R`, korektní ukončení klávesou `Q`. |

---

## 7. Citace použitých zdrojů

1. **Původní hra 2048:** Cirulli, Gabriele. *2048 original game concept and mechanics*. Dostupné online: [https://github.com/gabrielecirulli/2048](https://github.com/gabrielecirulli/2048).
2. **Standard C++:** ISO/IEC 14882:2020 – *Programming Language C++ (C++20)*. Dokumentace na [cppreference.com](https://en.cppreference.com/).
3. **Terminálové rozhraní POSIX:** IEEE Std 1003.1-2017 – *termios API, canonical and non-canonical mode*. Man termios(3).
4. **ANSI Escape Codes:** Standard ECMA-48 – *Control Functions for Coded Character Sets*, 5th edition (1991).
5. **Kurz B6B36PCC:** ČVUT v Praze, Fakulta elektrotechnická. *Podklady k přednáškám, cvičením a semestrální práci*. Dostupné na CourseWare ČVUT: [https://cw.fel.cvut.cz/wiki/courses/b6b36pcc/](https://cw.fel.cvut.cz/wiki/courses/b6b36pcc/).
6. **Využití nástrojů umělé inteligence:** V souladu s pravidly zadání semestrální práce byl při vývoji využit asistenční model AI (Google Antigravity / Gemini). Nástroj byl využit pro:
   * konzultaci syntaxe a konfigurace terminálového rozhraní POSIX `termios` (přepínání do raw módu a ANSI escape sekvence),
   * asistenci s konfigurací sestavení v `CMakeLists.txt` pro integraci a spouštění testů (CTest),
   * návrh doplňkových testovacích scénářů a vzorových vstupů pro ověření hraničních stavů,
   * formátování a jazykovou korekturu technické dokumentace.
