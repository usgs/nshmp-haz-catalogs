package gov.usgs.earthquake.catalog;

import java.io.IOException;
import java.nio.file.Path;

import gov.usgs.earthquake.catalog.config.AgridConfig;

public class CEUSAgridCalc {
  private static final String USAGE_COMMAND = AgridUtils.JAVA_COMMAND + 
      " agrid.CEUSAgridCalc catalogFile [config]";
  
  public static void main(String[] args) throws IOException, InterruptedException {
    AgridUtils.checkArguments(args, USAGE_COMMAND);
    
    try {
      AgridModel agridModel = AgridModel.getAgridModel(args);
      AgridConfig config = agridModel.config;
      
      CEUSAdaptiveCalc.runAdaptive(agridModel);
      CEUSFixedCalc.runFixed(agridModel);

      Path out = config.output.directory.resolve("config.json"); 
      config.write(out);
      System.out.printf("\n\nFiles reside in [%s] \n\n", config.output.directory);
    } catch (Exception e) {
      AgridUtils.handleError(e, args, USAGE_COMMAND);
    }
    
    System.exit(0);
  }
  
}
