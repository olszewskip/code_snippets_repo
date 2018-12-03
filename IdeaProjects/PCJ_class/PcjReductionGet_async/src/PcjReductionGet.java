import java.io.IOException;
import org.pcj.*;

@RegisterStorage(PcjReductionGet.Shared.class)
public class PcjReductionGet implements StartPoint {

  @Storage(PcjReductionGet.class)
  enum Shared { a }
  long a;

  public static void main(String[] args) throws IOException {
    String nodesFile = "nodes.txt";
    PCJ.deploy(PcjReductionGet.class, new NodesDescription("nodes.txt"));
  }

  @Override
  public void main() throws Throwable {
    PcjFuture aL[] = new PcjFuture[PCJ.threadCount()];
    PCJ.barrier();
    long sum;
    if (PCJ.myId() == 0) {         // Asynchronous communication
      for (int p = 0; p < PCJ.threadCount(); p++) {
        aL[p] = PCJ.asyncGet(p, Shared.a);
      }
    }
    PCJ.barrier();                 // Synchronization
    sum = 0;
    if (PCJ.myId() == 0) {         // Sumation of local values
      for (int p = 0; p < PCJ.threadCount(); p++) {
        sum = sum + (long) aL[p].get();
      }
    }
    System.out.println(PCJ.myId() + " " + sum);
  }
}