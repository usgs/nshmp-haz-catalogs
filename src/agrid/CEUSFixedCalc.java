package agrid;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Path;

import agrid.CEUSAgridCalc.AgridArguments;
import agrid.config.AgridConfig;
import agrid.config.FixedConfig;

public class CEUSFixedCalc {

  public static void main(String[] args) throws IOException, InterruptedException {
    AgridArguments agridArgs = AgridArguments.getArguments(args);
    AgridConfig config = agridArgs.config;
    Path catalogFilePath = agridArgs.catalogFilePath;
    
    runFixed(config, catalogFilePath);
    Path out = config.output.directory.resolve("config.json"); 
    FixedConfig.writeFixedConfig(config, out);
    System.out.printf("\n\nFiles reside in [%s] \n\n", config.output.directory);
  }

  static void runFixed(
      AgridConfig config,
      Path catalogFile) throws IOException, InterruptedException {
    System.out.println("\n\n============= Running Fixed ============== \n");

    Path fixedDirectoryPath = config.output.directory.resolve("fixed")
        .toAbsolutePath()
        .normalize();
   
    AgridUtils.createDirectory(fixedDirectoryPath);

    Path modelInput = config.fixed.completenessModel.writeModelInput(fixedDirectoryPath);
    modelInput = modelInput.getFileName();
  
    config.catalog.sourceBoundaries.writeFixedGridFile(fixedDirectoryPath);
    
    String cmd = String.format(
        "%s %s %s %d %f %f %d",
        CEUSAgridCalc.FIXED_EXE_PATH,
        catalogFile,
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
