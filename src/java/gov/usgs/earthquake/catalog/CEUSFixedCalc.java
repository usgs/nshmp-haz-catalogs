package gov.usgs.earthquake.catalog;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Path;

import gov.usgs.earthquake.catalog.config.AgridConfig;
import gov.usgs.earthquake.catalog.config.FixedConfig;

public class CEUSFixedCalc {
  private static final String USAGE_COMMAND = AgridUtils.JAVA_COMMAND + 
      " agrid.CEUSFixedCalc catalogFile [config]";

  public static void main(String[] args) throws IOException, InterruptedException {
    AgridUtils.checkArguments(args, USAGE_COMMAND);
    
    try {
      AgridModel agridModel = AgridModel.getAgridModel(args);
      AgridConfig config = agridModel.config;
      
      runFixed(agridModel);
      Path out = config.output.directory.resolve("config.json"); 
      FixedConfig.writeFixedConfig(config, out);
      System.out.printf("\n\nFiles reside in [%s] \n\n", config.output.directory);
    } catch (Exception e) {
      AgridUtils.handleError(e, args, USAGE_COMMAND);
    }
    
    System.exit(0);
  }

  static void runFixed(AgridModel agridModel) throws IOException, InterruptedException {
    System.out.println("\n\n============= Running Fixed ============== \n");

    AgridConfig config = agridModel.config;
    Path catalogFilePath = agridModel.catalogFilePath;
    
    Path fixedDirectoryPath = config.output.directory.resolve("fixed")
        .toAbsolutePath()
        .normalize();
   
    AgridUtils.createDirectory(fixedDirectoryPath);

    Path modelInput = config.fixed.completenessModel.writeModelInput(fixedDirectoryPath);
    modelInput = modelInput.getFileName();
  
    config.catalog.sourceBoundaries.writeFixedGridFile(fixedDirectoryPath);
    
    String cmd = String.format(
        "%s %s %s %d %f %f %d",
        agridModel.fixedExePath,
        catalogFilePath,
        modelInput,
        config.catalog.catalogEndYear,
        config.catalog.bValue,
        config.fixed.smoothingCorrelationLength,
        AgridUtils.boolean2Int(config.fixed.forceUniformEtennzNewmadRates));
    
    System.out.println("Executing: " + cmd + "\n\n");
    Process runProgram = new ProcessBuilder()
        .redirectErrorStream(true)
        .command("bash", "-c", cmd)
        .directory(fixedDirectoryPath.toFile())
        .start();
    
    BufferedReader reader = new BufferedReader(new InputStreamReader(runProgram.getInputStream()));
    String line = null;
    while((line = reader.readLine()) != null) {
      System.out.println(line);
    }
    reader.close();
    runProgram.waitFor();
  }
  
}
