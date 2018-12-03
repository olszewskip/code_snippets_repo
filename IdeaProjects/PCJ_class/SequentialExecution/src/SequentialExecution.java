import java.io.IOException;
import org.pcj.*;

public class SequentialExecution implements StartPoint {

  public static void main(String[] args) throws IOException {
    String nodesFile = "nodes.txt";

    PCJ.deploy(SequentialExecution.class, new NodesDescription("nodes.txt"));
  }

  @Override
  public void main() throws Throwable {
    PCJ.barrier();
    for (int p = 0; p < PCJ.threadCount(); p++) {
      if (PCJ.myId() == p) {
        System.out.println("Hello from " + PCJ.myId() + " of " + PCJ.threadCount());
      }
      PCJ.barrier();
    }
  }
}