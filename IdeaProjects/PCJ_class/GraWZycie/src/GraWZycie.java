import java.util.Scanner;

/**
 * Prosta implementacja Gry w życie
 * @author kodatnik.blogspot.com
 */
public class GraWZycie {

  // stałe symbolizujące żywą i martwą komórkę
  private final char ZYWA = '#';
  private final char MARTWA = '.';
  // pola przechowujące rozmiar naszej planszy
  private int wiersze;
  private int kolumny;
  // deklaracja macierzy z planszą gry
  private boolean[][] plansza;

  // konstruktor, podajemy liczbę wierszy i kolumn planszy
  public GraWZycie(int wiersze, int kolumny) {
    // inicjalizujemy pola naszej klasy
    this.wiersze = wiersze;
    this.kolumny = kolumny;
    // tworzymy odpowiednią macierz, domyślnie wypełniona jest ona wartościami false
    plansza = new boolean[wiersze][kolumny];
  }

  // metoda pokazująca planszę na ekranie
  public void pokazPlansze(){
    // wyświetlamy każdy element planszy, w zależności od zawartości na ekranie pojawi się odpowiedni znak
    for(boolean wiersz[] : plansza){
      for(boolean element : wiersz)
        System.out.print((element) ? ZYWA : MARTWA);
      System.out.println();
    }
  }

  // przykładowa metoda uzupełniająca planszę populacją początkową
  public void utworzDaneTestowe() {
    // tworzymy populację początkową (standardowy Exploder)
    plansza[5][5] = true; plansza[5][7] = true; plansza[5][9] = true;
    plansza[6][5] = true; plansza[6][9] = true;
    plansza[7][5] = true; plansza[7][9] = true;
    plansza[8][5] = true; plansza[8][9] = true;
    plansza[9][5] = true; plansza[9][7] = true; plansza[9][9] = true;
  }

  // metoda zwraca liczbę żywych sąsiadów konkretnej komórki
  // w wywołaniu podajemy współrzędne komórki
  private int liczbaSasiadow(int x, int y) {

    // zmienna przechowująca liczbę sąsiadów
    int sasiedzi = 0;

    // sprawdzamy komórki na około naszej
    for(int i = x-1; i <= x+1; i++) {
      // jeżeli przekraczamy zakres planszy (wiersz) idziemy dalej
      if(i < 0 || i > wiersze-1) continue;
      for(int j = y-1; j <= y+1; j++) {
        // jeżeli przekraczamy zakres planszy (kolumna), lub jesteśmy w komórce(x,y) idziemy dalej
        if(j < 0 || j > kolumny-1 || (i == x && j == y)) continue;
        // jeśli sąsiad jest żywy to zwiększamy naszą zmienną
        if(plansza[i][j]) sasiedzi++;
      }
    }
    // zwracamy zmienną
    return sasiedzi;
  }

  // metoda sprawdza czy dana komórka będzie żywa czy też martwa w następnym etapie
  // w wywołaniu podajemy współrzędne komórki
  // DO ZAIMPLEMENTOWANIA SAMODZIELNIE
  private boolean ewolucja(int x, int y) {

    // sprawdzamy liczbę żywych sąsiadów
    int sasiedzi = liczbaSasiadow(x, y);

    // jeżeli nasza komórka jest żywa
    // jeśli liczba sąsiadów jest mniejsza lub równa jeden lub większa od trzech to nasza komórka będzie martwa
    // jeśli liczba sąsiadów jest równa trzy lub dwa to nasza komórka będzie żywa
    // jeśli nasza komórka jest martwa i ma dokładnie trzech żywych sąsiadów to będzie żyła
    // w każdym innym przypadku zwracamy fałsz (komórka jest martwa)
    return false;
  }

  // metoda tworzy następną populację/pokolenie komórek
  public void nastepnePokolenie() {
    // tworzymy planszę tymczasową o takim samy rozmiarze jak nasza
    boolean[][] planszaTymczasowa = new boolean[wiersze][kolumny];

    // uzupełniamy nową planszę wartościami na podstawie ogólnych zasad
    for(int i = 0; i < wiersze; i++)
      for(int j = 0; j < kolumny; j++)
        planszaTymczasowa[i][j] = ewolucja(i, j);

    // nasza główna plansza staje się planszą tymczasową
    plansza = planszaTymczasowa;
  }

  public static void main(String[] args) {

    // wyświetlamy napisy informacyjne
    System.out.println("Gra w życie - [Enter] generuje następne pokolenie, dowolny znak i [Enter] kończy grę.");

    // obsługujemy wejście
    Scanner sc = new Scanner(System.in);

    // tworzymy nowy obiekt przekazując do konstruktora rozmiar planszy
    GraWZycie gra = new GraWZycie(15, 15);
    // wypełniamy naszą planszę wartościami testowymi
    gra.utworzDaneTestowe();
    // wyświetlamy planszę na ekranie
    gra.pokazPlansze();

    // dopóki użytkownik będzie naciskał klawisz Enter
    while(sc.nextLine().equals("")) {
      // generujemy następne pokolenie
      gra.nastepnePokolenie();
      // wyświetlamy go na ekranie
      gra.pokazPlansze();
    }
  }
}