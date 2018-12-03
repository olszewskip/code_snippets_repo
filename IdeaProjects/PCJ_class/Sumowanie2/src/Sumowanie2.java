import java.io.IOException;
import org.pcj.NodesDescription;
import org.pcj.PCJ;
import org.pcj.RegisterStorage;
import org.pcj.StartPoint;
import org.pcj.Storage;

/**
 * @author pbala
 */
@RegisterStorage(Sumowanie2.Shared.class)
public class Sumowanie2 implements StartPoint {

  @Storage(Sumowanie2.class)
  enum Shared {
    suma
  };
  double suma = 0.0;

  double c[] = new double[1024];

  public static void main(String[] args) throws IOException {

//        String[] nodes = {"localhost","localhost", "localhost", "localhost"};  
//        NodesDescription nd = new NodesDescription(nodes); 
    NodesDescription nd = new NodesDescription("nodes.txt");
    PCJ.deploy(Sumowanie2.class, nd);
  }

  @Override
  public void main() throws Throwable {

    int n = 1024;
    int np = n / PCJ.threadCount();

    for (int i = 0; i < np; i++) {                     // Dane rozproszone
      c[i] = PCJ.myId() * np + i;                 // Każdy procesor ma tylko dane które sumuje
    }

    for (int i = 0; i < np; i++) {
      suma = suma + c[i];
    }

    PCJ.barrier();

    System.out.println("form " + PCJ.myId() + " +++ " + suma);  // sumy częsiowe

    if (PCJ.myId() == 0) {                                                              // Procesor 0 pobiera sumy częściowe z wszystkich
      for (int p = 1; p < PCJ.threadCount(); p++) {
        suma = suma + (double) PCJ.get(p, Shared.suma);
      }
      System.out.println("suma =  " + suma);
    }
  }
}