/**
 * @author pbala
 */
public class Lancuchy {

  public static void main (String[] args){

    String s = "Ala  ma kota";

    System.out.println(s.indexOf("a"));
    System.out.println(s.lastIndexOf("a"));

    s = "4.0";
    double a;
    a = Double.parseDouble(s);
    a = a * a ;
    System.out.println(" a * a = "+a);

    int b = 5;
    a = (double) b;
    System.out.println(" a = "+a);
  }
}