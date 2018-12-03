import org.pcj.*;

import java.io.IOException;
import java.util.Random;

@RegisterStorage(PCJWygladzanie.Shared.class)
public class PCJWygladzanie implements StartPoint {

  @Storage(PCJWygladzanie.class)
  enum Shared {
    x_left, x_right
  }

  double x_left[] = new double[2];
  double x_right[] = new double[2];


  public static void main(String[] args) throws IOException {
    PCJ.deploy(PCJWygladzanie.class, new NodesDescription("nodes.txt"));
  }

  @Override
  public void main() {

    int nAll = 1024;
    int n = nAll / PCJ.threadCount();

    double x[] = new double[n];
    double y[] = new double[n];

    double a[] = {-3, 12, 17, 12, -3};
    double d = 1.0 / (a[0] + a[1] + a[2] + a[3] + a[4]);

    Random r = new Random();

    for (int i = 0; i < n; i++) {

      x[i] = r.nextDouble();

    }

    // komunikacja i->i+1
    if (PCJ.myId() < PCJ.threadCount() - 1) {

      PCJ.asyncPut(x[n - 2], PCJ.myId() + 1, Shared.x_left, 0);
      PCJ.asyncPut(x[n - 1], PCJ.myId() + 1, Shared.x_left, 1);

    }

    // komunikacja i->i-1
    if (PCJ.myId() > 0) {

      PCJ.put(x[n - 2], PCJ.myId() - 1, Shared.x_right, 0);
      PCJ.put(x[n - 1], PCJ.myId() - 1, Shared.x_right, 1);

    }

    //oliczenia
    for (int i = 2; i < n - 2; i++) {

      y[i] = a[0] * x[i - 2] + a[1] * x[i - 1] + a[2] * x[i] + a[3] * x[i + 1] + a[4] * x[i + 2];
      y[i] = y[i] * d;

    }

    //waitfor
    if (PCJ.myId() > 0) {
      PCJ.waitFor(Shared.x_left, 2);
    }

    if (PCJ.myId() < PCJ.threadCount() - 1) {
      PCJ.waitFor(Shared.x_right, 2);
    }

    System.out.println(PCJ.myId() + " " + x_left[0] + " " + x_left[1] + " " + x_right[0] + " " + x_right[1]);

    if (PCJ.myId() == 0) {
      y[0] = a[2] * x[0] + a[3] * x[1] + a[4] * x[2];
      y[0] = y[0] / (a[2] + a[3] + a[4]);
      y[1] = a[1] * x[0] + a[2] * x[1] + a[3] * x[2] + a[4] * x[3];
      y[1] = y[1] / (a[1] + a[2] + a[3] + a[4]);
    }
    else {
      y[0] = a[0] * x_left[0] + a[1] * x_left[1] + a[2] * x[0] + a[3] * x[1] + a[4] * x[2];
      y[0] = y[0] * d;
      y[1] = a[0] * x_left[1] + a[1] * x[0] + a[2] * x[1] + a[3] * x[2] + a[4] * x[3];
      y[1] = y[1] * d;
    }

    if (PCJ.myId() == PCJ.threadCount() - 1) {
      y[n - 2] = a[0] * x[n - 4] + a[1] * x[n - 3] + a[2] * x[n - 2] + a[3] * x[n - 1];
      y[n - 2] = y[n - 2] / (a[0] + a[1] + a[2] + a[3]);
      y[n - 1] = a[0] * x[n - 3] + a[1] * x[n - 2] + a[2] * x[n - 1];
      y[n - 1] = y[n - 1] / (a[0] + a[1] + a[2]);
    }
    else {
      y[n - 2] = a[0] * x[n - 4] + a[1] * x[n - 3] + a[2] * x[n - 2] + a[3] * x[n - 1] + a[4] * x_right[0];
      y[n - 2] = y[n - 2] * d;
      y[n - 1] = a[0] * x[n - 3] + a[1] * x[n - 2] + a[2] * x[n - 1] + a[3] * x_right[0] + a[4] * x_right[1];
      y[n - 1] = y[n - 1] * d;
    }

    for (int p=0; p<PCJ.threadCount(); p++) {

      if (PCJ.myId() == p) {

        System.out.println(PCJ.myId() + ")");

        for (int i = 0; i < n; i++) {

          System.out.println((PCJ.myId() * n + i) + "\t" + x[i] + "\t" + y[i]);

        }

      }

      PCJ.barrier();

    }

  }

}
