import java.io.IOException;
import org.pcj.NodesDescription;
import org.pcj.PCJ;
import org.pcj.StartPoint;

/**
 *
 * @author pbala
 */
public class HelloWorld implements StartPoint {

  public static void main(String[] args) throws IOException {

//  Opis węzłów za pomocą String[]
//        String[] nodes = {"localhost","localhost", "localhost", "localhost"};
//        NodesDescription nd = new NodesDescription(nodes);


// Lista węzłów wczytywana z pliku nodes.txt (plik musi być w folderze głównym projektu)
    NodesDescription nd = new NodesDescription("nodes.txt");
    PCJ.deploy(HelloWorld.class, nd);
  }

  @Override
  public void main() throws Throwable {
    System.out.println("Hello world form " + PCJ.myId() + " out of "+ PCJ.threadCount());
  }
}