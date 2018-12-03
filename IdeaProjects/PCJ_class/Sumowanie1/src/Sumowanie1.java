import java.io.IOException;
import org.pcj.NodesDescription;
import org.pcj.PCJ;
import org.pcj.RegisterStorage;
import org.pcj.StartPoint;
import org.pcj.Storage;

/**
 * @author pbala
 */
@RegisterStorage(Sumowanie1.Shared.class)
public class Sumowanie1 implements StartPoint {

  @Storage(Sumowanie1.class)
  enum Shared {
    suma
  };
  double suma = 0.0;

  double c[] = new double[1024];

  public static void main(String[] args) throws IOException {

//        String[] nodes = {"localhost","localhost", "localhost", "localhost"};  
//        NodesDescription nd = new NodesDescription(nodes); 
    NodesDescription nd = new NodesDescription("nodes.txt");
    PCJ.deploy(Sumowanie1.class, nd);
  }

  @Override
  public void main() throws Throwable {

    int n = 1024;                                    // Dane pozostają w pamięci w jednym miesjcu
    for (int i = 0; i < n; i++) {                 // Wszystmkie dane skopiowane na procesorach
      c[i] = i;
    }

    int np = n / PCJ.threadCount();        // ilość elementów sumowanych przez wątek
    int ip = PCJ.myId() * np;                   // początek zakresu sumowania
    int ik = (PCJ.myId() + 1) * np;           // koniec zakresu sumowania

    for (int i = ip; i < ik; i++) {
      suma = suma + c[i];
    }
    PCJ.barrier();

    System.out.println("form " + PCJ.myId() + " +++ " + suma);

    if (PCJ.myId() == 0) {
      for (int p = 1; p < PCJ.threadCount(); p++) {
        suma = suma + (double) PCJ.get(p, Shared.suma);
      }
      System.out.println("suma =  " + suma);
    }
  }
}