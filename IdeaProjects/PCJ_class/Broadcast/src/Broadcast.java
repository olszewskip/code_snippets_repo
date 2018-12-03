import java.io.IOException;
import org.pcj.NodesDescription;
import org.pcj.PCJ;
import org.pcj.RegisterStorage;
import org.pcj.StartPoint;
import org.pcj.Storage;

/**
 *
 * @author pbala
 */
@RegisterStorage(Broadcast.Shared.class)
public class Broadcast implements StartPoint {

  @Storage(Broadcast.class)             // Zmienna współdzielona
  enum Shared {
    a
  };
  double a = 0.0;

  public static void main(String[] args) throws IOException {

    NodesDescription nd = new NodesDescription("nodes.txt");
    PCJ.deploy(Broadcast.class, nd);
  }

  @Override
  public void main() throws Throwable {
    System.out.println("a form " + PCJ.myId() + " --- " + a);
    PCJ.monitor(Shared.a);
    PCJ.asyncBroadcast(1.0, Shared.a);
    PCJ.waitFor(Shared.a);
    System.out.println("a form " + PCJ.myId() + " +++ " + a);

    System.out.println("a form " + PCJ.myId() + " --- " + a);
    PCJ.broadcast(2.0, Shared.a);
    System.out.println("a form " + PCJ.myId() + " +++ " + a);
  }
}