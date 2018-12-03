import org.pcj.*;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * @author pbala
 */

@RegisterStorage(PrzewodnictwoPCJ.Shared.class)
public class PrzewodnictwoPCJ implements StartPoint{

  @Storage(PrzewodnictwoPCJ.class)
  enum Shared{T};
  double T[][];


  public static void main(String[] args) throws IOException {

    PCJ.deploy(PrzewodnictwoPCJ.class, new NodesDescription("nodes.txt"));

  }

  @Override
  public void main() {

    int np = (int) Math.sqrt((float) PCJ.threadCount());   // np * np == PCH.threadCount()
    int N = np * np;

// Material
    double rho = 7800;              // density
    double c = 500;                 // heat capacity
    double lambda = 46;             // thermal conductivity

    double a = lambda / (rho * c);  //

// Parametry płytki AxB
    double A = 0.32;            // height
    double B = 1.6;             // width
    double t = 1000;            // time

// Bordering
    double T0 = 20;             // initial temperature
    double hx = 0.01;           // x step
    double hy = 0.01;           // y step
    double tau = 1 / (a * 12 * (1 / (hx * hx) + 1 / (hy * hy)));     // time step (calculated as maximal step of explicit method)

    int Nx = Math.round((float) (B / (hx)));             // size of x grid
    int Ny = Math.round((float) (A / (hy)));             // size of y grid
    int Nt = Math.round((float) (t / tau));              // number of time steps

    T = new double[Nx][Ny];                      // Temperature at time k
    double Tnew[][] = new double[Nx][Ny];        // Temperature at time k+1
    // Set initial temperature
    for (int i = 0; i < Nx; i++) {
      for (int j = 0; j < Ny; j++) {
        T[i][j] = T0;
      }
    }

    // Border conditions (constatnt temperature on borders)
    double Tl = 1200;
    double Tr = 1200;
    double Tu = 1200;
    double Td = 1200;

    for (int k = 0; k < Nt; k++) {

      for (int i = 0; i < Nx; i++) {
        for (int j = 0; j < Ny; j++) {

          if (i == 0 && j == 0) {
            if (PCJ.myId() == 0) {                   // fizyczny róg lewy górny
              Tnew[i][j] = T[i][j] + 2 * a * tau * (((T[i + 1][j] + T[i][j + 1] - 2 * T[i][j]) / (hx * hx)) + (Tl + Td) / (lambda * hx));
            }
            else if (PCJ.myId() < np) {              // fizyczny brzeg górny
              double Tpom = (double) PCJ.get(PCJ.myId() - 1, Shared.T, Nx - 1, j);
              Tnew[i][j] = T[i][j] + a * tau * (T[i + 1][j] - 2 * T[i][j] + Tpom) + (Td + (lambda * (T[i][j + 1] - T[i][j]) / hx)) * (2 * tau / (c * rho * hx));
            }
            else if (PCJ.myId() % np == 0) {    // fizyczny brzeg lewy
              double Tpom = (double) PCJ.get(PCJ.myId() - np, Shared.T, i, Ny - 1);
              Tnew[i][j] = T[i][j] + a * tau * (T[i][j + 1] - 2 * T[i][j] + Tpom) + (Tl + (lambda * (T[i + 1][j] - T[i][j]) / hx)) * (2 * tau / (c * rho * hx));
            }
            else {                                                  // mamy sąsiadów z lewa i zgóry
              double Tleft = (double) PCJ.get(PCJ.myId() - 1, Shared.T, Nx - 1, j);
              double Ttop = (double) PCJ.get(PCJ.myId() - np, Shared.T, i, Ny - 1);
              Tnew[i][j] = T[i][j] + a * tau * ((T[i + 1][j] - 2 * T[i][j] + Tleft) / (hx * hx) + (T[i][j + 1] - 2 * T[i][j] + Ttop) / (hy * hy));
            }
          }

          else if (i == 0 && j == Ny - 1) {
            if (PCJ.myId() == np - 1) {
              Tnew[i][j] = T[i][j] + 2 * a * tau * (((T[i + 1][j] + T[i][j - 1] - 2 * T[i][j]) / (hx * hx)) + (Tl + Tu) / (lambda * hx));
            }
          }

          else if (i == Nx - 1 && j == 0) {
            if (PCJ.myId() == PCJ.threadCount() - np) {
              Tnew[i][j] = T[i][j] + 2 * a * tau * (((T[i - 1][j] + T[i][j + 1] - 2 * T[i][j]) / (hx * hx)) + (Tr + Td) / (lambda * hx));
            }
          }

          else if (i == Nx - 1 && j == Ny - 1) {
            if (PCJ.myId() == PCJ.threadCount() - 1) {
              Tnew[i][j] = T[i][j] + 2 * a * tau * (((T[i - 1][j] + T[i][j - 1] - 2 * T[i][j]) / (hx * hx)) + (Tr + Tu) / (lambda * hx));
            }
          }

          else if (i == 0) {                       // Left  border
            if (PCJ.myId() % np == 0) { // fizyvczny brzeg
              Tnew[i][j] = T[i][j] + a * tau * (T[i][j + 1] - 2 * T[i][j] + T[i][j - 1]) + (Tl + (lambda * (T[i + 1][j] - T[i][j]) / hx)) * (2 * tau / (c * rho * hx));
            } else {                                    // jest procesor z lewej
              double Tpom = (double) PCJ.get(PCJ.myId() - 1, Shared.T, Nx - 1, j);
              Tnew[i][j] = T[i][j] + a * tau * ((T[i + 1][j] - 2 * T[i][j] + Tpom) / (hx * hx) + (T[i][j + 1] - 2 * T[i][j] + T[i][j - 1]) / (hy * hy));
            }
          }

          else if (i == Nx - 1) {              // Right  border
            Tnew[i][j] = T[i][j] + a * tau * (T[i][j + 1] - 2 * T[i][j] + T[i][j - 1]) + (Tr + (lambda * (T[i - 1][j] - T[i][j]) / hx)) * (2 * tau / (c * rho * hx));
          }

          else if (j == 0) {                       // Top border
            Tnew[i][j] = T[i][j] + a * tau * (T[i + 1][j] - 2 * T[i][j] + T[i - 1][j]) + (Td + (lambda * (T[i][j + 1] - T[i][j]) / hx)) * (2 * tau / (c * rho * hx));
          }

          else if (j == Ny - 1) {              // Bottom border
            Tnew[i][j] = T[i][j] + a * tau * (T[i + 1][j] - 2 * T[i][j] + T[i - 1][j]) + (Tu + (lambda * (T[i][j - 1] - T[i][j]) / hx)) * (2 * tau / (c * rho * hx));
          }

          else {                                      // internal points
            Tnew[i][j] = T[i][j] + a * tau * ((T[i + 1][j] - 2 * T[i][j] + T[i - 1][j]) / (hx * hx) + (T[i][j + 1] - 2 * T[i][j] + T[i][j - 1]) / (hy * hy));
          }

        }
      }

      System.out.println(k);
      PCJ.barrier();
      T = Tnew;             // Update temperature table (k -> k+1)

    }

    // output to file
    Writer out;

    double S[] = new double[Ny];
    // for i
    // for j
    // T[i][j]

    if (PCJ.myId() == 0) {

      try {
        String file = "out.txt";
        out = new FileWriter(file);

        for (int py = 0; py < np; py++) {
          for (int i = 0; i < Nx; i++) {

            for (int px = 0; px < np; px++) {
              for (int j = 0; j < Ny; j++) {
                out.write((double) PCJ.get(px+np*py, Shared.T, i, j) + " ");
              }
            }

            out.write("\n");

          }
        }

        out.close();
      }
      catch(IOException ex){
        Logger.getLogger(PrzewodnictwoPCJ.class.getName()).log(Level.SEVERE, null, ex);
      }

    }

  }
}