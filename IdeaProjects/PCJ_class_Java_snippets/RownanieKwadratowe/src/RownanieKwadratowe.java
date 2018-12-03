import java.util.Scanner;

/**
 * @author pbala
 */
public class RownanieKwadratowe {

  public static void main(String[] args) {

    double a = 1.0;
    double b = 0;
    double c = -4;

    Scanner sc = new Scanner(System.in);
    System.out.print(" Podaj współczynniki r. kwadratowego: ");
    a = sc.nextDouble();
    b = sc.nextDouble();
    c = sc.nextDouble();

    double d = delta(a, b, c);

    if (d < 0) {
      System.out.println(" Nie ma pierwiastków rzeczywistych");
    }

    if (d == 0) {
      double x = -b / (2 * a);
      System.out.println(" Jeden pierwiastek rzeczywisty: " + x);
    }

    if (d > 0) {
      double x1 = (-b + Math.sqrt(d)) / (2 * a);
      double x2 = (-b - Math.sqrt(d)) / (2 * a);
      System.out.println(" Dwa pierwiastki rzeczywiste: " + x1 + " " + x2);
    }
  }

  static double delta(double a, double b, double c){
    double d = b * b - 4 * a * c;
    return d;
  }
}