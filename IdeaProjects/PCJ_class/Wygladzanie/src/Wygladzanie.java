import java.util.Random;

public class Wygladzanie {

  public static void main(String[] args) {

    int n = 1024;
    double x[] = new double[n];
    double y[] = new double[n];

    double a[] = {-3, 12, 17, 12, -3};
    double d = 1.0 / (a[0] + a[1] + a[2] + a[3] + a[4]);

    Random r = new Random();

    for (int i = 0; i < n; i++) {

      x[i] = r.nextDouble();

    }

    y[0] = a[2] * x[0] + a[3] * x[1] + a[4] * x[2];
    y[0] = y[0] / (a[2] + a[3] + a[4]);

    y[1] = a[1] * x[0] + a[2] * x[1] + a[3] * x[2] + a[4] * x[3];
    y[1] = y[1] / (a[1] + a[2] + a[3] + a[4]);

    for (int i=2; i<n-2; i++) {

      y[i] = a[0] * x[i-2] + a[1] * x[i-1] + a[2] * x[i] + a[3] * x[i+1] + a[4] * x[i+2];
      y[i] = y[i] * d;

    }

    y[n-2] = a[0] * x[n-4] + a[1] * x[n-3] + a[2] * x[n-2] + a[3] * x[n-1];
    y[n-2] = y[n-2] / (a[0] + a[1] + a[2] + a[3]);

    y[n-1] = a[0] * x[n-3] + a[1] * x[n-2] + a[2] * x[n-1];
    y[n-1] = y[n-1] / (a[0] + a[1] + a[2]);

    for (int i = 0; i < n; i++) {

      System.out.println("" + i + ") " + x[i] + " , " + y[i]);

    }

  }

}
