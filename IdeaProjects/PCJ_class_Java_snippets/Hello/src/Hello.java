/**
 * @author pbala
 */
public class Hello {

  static int a = 0;
  static double b;

  public static void main(String[] args) {
    System.out.println("Hello !");     // Komentarz

    a = 3;
    b = 2 * a;

    System.out.println(a + " " +b);
    System.out.println(a +b + " ");

    double array[];
    array = new double[10];

    array[0] = 0;
    array[1] = 1;

    System.out.println(array[1]);
    System.out.println(array[2]);

    for (int i=0; i<array.length; i=i+1){
      array[i]  = 2*i;
    }

    for (int i=0; i<array.length; i=i+1){
      System.out.println(i + " " + array[i]);
    }

    double suma = 0;
    for (int i=0; i<array.length; i=i+1){
      suma = suma + array[i];
    }
    System.out.println( "suma  " + suma);
  }
}