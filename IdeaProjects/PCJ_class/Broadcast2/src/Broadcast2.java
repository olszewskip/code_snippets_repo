import java.io.IOException;
import java.util.Scanner;
import org.pcj.NodesDescription;
import org.pcj.PCJ;
import org.pcj.RegisterStorage;
import org.pcj.StartPoint;
import org.pcj.Storage;

/**
 *
 * @author pbala
 */
@RegisterStorage(Broadcast2.Shared.class)
public class Broadcast2 implements StartPoint {

  @Storage(Broadcast2.class)
  enum Shared {
    a
  };
  double a = 0.0;

  public static void main(String[] args) throws IOException {

//        String[] nodes = {"localhost","localhost", "localhost", "localhost"};  
//        NodesDescription nd = new NodesDescription(nodes); 
    NodesDescription nd = new NodesDescription("nodes.txt");
    PCJ.deploy(Broadcast2.class, nd);
  }

  @Override
  public void main() throws Throwable {

    PCJ.monitor(Shared.a);
    if (PCJ.myId() == 0) {
      Scanner sc = new Scanner(System.in);
      double aTemp = sc.nextDouble();
      PCJ.broadcast(aTemp, Shared.a);
    }
    PCJ.waitFor(Shared.a);
    System.out.println("a form " + PCJ.myId() + " --- " + a);
  }
}