import java.io.IOException;
import org.pcj.*;

@RegisterStorage(PcjReductionGet.Shared.class)
public class PcjReductionGet implements StartPoint {

  @Storage(PcjReductionGet.class)
  enum Shared { a }
  long a;

  public static void main(String[] args) throws IOException {
    String nodesFile  = "nodes.txt";
    PCJ.deploy(PcjReductionGet.class, new NodesDescription("nodes.txt"));
  }

  @Override
  public void main() throws Throwable {
    a = PCJ.myId() + 10;    // set value of a
    long sum = 0;
    PCJ.barrier();
    if (PCJ.myId() == 0) {
      for (int p = 1; p < PCJ.threadCount(); p++) {
        sum = sum + (long) PCJ.get(p, Shared.a);
      }
    }
    PCJ.barrier();
    System.out.println(PCJ.myId()+ " "+ sum);
  }
}