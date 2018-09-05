package gov.usgs.earthquake.catalog;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Path;

import gov.usgs.earthquake.catalog.config.AdaptiveConfig;
import gov.usgs.earthquake.catalog.config.AgridConfig;
import gov.usgs.earthquake.catalog.config.WriteAdaptiveFiles;

public class CEUSAdaptiveCalc {
  private static final String USAGE_COMMAND = AgridUtils.JAVA_COMMAND + 
      " agrid.CEUSAdaptiveCalc catalogFile [config]";
  private static final String PARAMETER_FILE = "agridParameters.in";
  private static final String SUMMARY_FILE = "agridParameterSummary.txt";
  

  public static void main(String[] args) throws IOException, InterruptedException {
    AgridUtils.checkArguments(args, USAGE_COMMAND);
    
    try {
      AgridModel agridModel = AgridModel.getAgridModel(args);
      AgridConfig config = agridModel.config;

      runAdaptive(agridModel);
      Path out = config.output.directory.resolve("config.json"); 
      AdaptiveConfig.writeAdaptiveConfig(config, out);
      System.out.printf("\n\nFiles reside in [%s] \n\n", config.output.directory);
    } catch (Exception e) {
      AgridUtils.handleError(e, args, USAGE_COMMAND);
    }
    
    System.exit(0);
  }

  static void runAdaptive(AgridModel agridModel) throws IOException, InterruptedException {
    System.out.println("\n\n============= Running Adaptive ============== \n");

    AgridConfig config = agridModel.config;
    
    Path adaptiveDirectoryPath = config.output.directory.resolve("adaptive")
        .toAbsolutePath()
        .normalize();
   
    AgridUtils.createDirectory(adaptiveDirectoryPath);
    
    Path parameterPath = WriteAdaptiveFiles.writeParameterFile(
        agridModel,
        adaptiveDirectoryPath,
        PARAMETER_FILE);
    parameterPath = parameterPath.getFileName();
    String paramFormat = "new";
    
    String cmd = String.format(
        "%s %s %s %s %d",
        agridModel.adaptiveExePath,
        parameterPath, 
        paramFormat, 
        SUMMARY_FILE,
        AgridUtils.boolean2Int(config.adaptive.smoothing.checkSmoothingParameters));

    System.out.println("Executing: " + cmd + "\n\n");
    Process runProgram = new ProcessBuilder()
        .redirectErrorStream(true)
        .command("bash", "-c", cmd)
        .directory(adaptiveDirectoryPath.toFile())
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
