import java.util.Random;
import org.pcj.NodesDescription;
import org.pcj.PCJ;
import org.pcj.StartPoint;
import org.pcj.Storage;
import org.pcj.PcjFuture;
import org.pcj.RegisterStorage;

@RegisterStorage(PcjExamplePiMC.Shared.class)
public class PcjExamplePiMC implements StartPoint {

  @Storage(PcjExamplePiMC.class)
  enum Shared { c }
  long c;

  @Override
  public void main() {
    PCJ.barrier();
    Random r = new Random();

    long nAll = 1000000;
    long n = nAll / PCJ.threadCount();
    double Rsq = 1.0;
    long circleCount;
//Calculate 
    circleCount = 0;
    double time = System.nanoTime();

    for (long i = 0; i < n; i++) {
      double x = 2.0 * r.nextDouble() - 1.0;
      double y = 2.0 * r.nextDouble() - 1.0;
      if ((x * x + y * y) < Rsq) {
        circleCount++;
      }
    }

    c = circleCount;
    PCJ.barrier();

// Communicate results 
    PcjFuture cL[] = new PcjFuture[PCJ.threadCount()];

    long c0 = c;
    if (PCJ.myId() == 0) {
      for (int p = 1; p < PCJ.threadCount(); p++) {
        cL[p] = PCJ.asyncGet(p, Shared.c);
      }
      for (int p = 1; p < PCJ.threadCount(); p++) {
        c0 = c0 + (long) cL[p].get();
      }
    }

    PCJ.barrier();

    double pi = 4.0 * (double) c0 / (double) nAll;
    time = System.nanoTime() - time;
// Print results 
    if (PCJ.myId() == 0) {
      System.out.println(pi + " " + time * 1.0E-9);
    }
  }

  public static void main(String[] args) {
    PCJ.deploy(PcjExamplePiMC.class, new NodesDescription("nodes.txt"));
  }
}