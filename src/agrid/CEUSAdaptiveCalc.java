package agrid;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Path;

import agrid.CEUSAgridCalc.AgridArguments;
import agrid.config.AdaptiveConfig;
import agrid.config.AgridConfig;
import agrid.config.WriteAdaptiveFiles;

public class CEUSAdaptiveCalc {

  public static void main(String[] args) throws IOException, InterruptedException {
    AgridArguments agridArgs = AgridArguments.getArguments(args);
    AgridConfig config = agridArgs.config;
    Path catalogFilePath = agridArgs.catalogFilePath;

    runAdaptive(config, catalogFilePath);
    Path out = config.output.directory.resolve("config.json"); 
    AdaptiveConfig.writeAdaptiveConfig(config, out);
    System.out.printf("\n\nFiles reside in [%s] \n\n", config.output.directory);
  }

  static void runAdaptive(
      AgridConfig config,
      Path catalogFilePath) throws IOException, InterruptedException {
    System.out.println("\n\n============= Running Adaptive ============== \n");

    Path adaptiveDirectoryPath = config.output.directory.resolve("adaptive")
        .toAbsolutePath()
        .normalize();
   
    AgridUtils.createDirectory(adaptiveDirectoryPath);
    
    WriteAdaptiveFiles.writeCatalogGapsFile(
        config, 
        adaptiveDirectoryPath);
    
    Path parameterPath = WriteAdaptiveFiles.writeParameterFile(
        config, 
        catalogFilePath, 
        adaptiveDirectoryPath);
    
    parameterPath = parameterPath.getFileName();
    
    String paramFormat = "new";
    String summaryFile = "smoothingParametersSummary.txt";
    
    String cmd = String.format(
        "%s %s %s %s",
        CEUSAgridCalc.ADAPTIVE_EXE_PATH,
        parameterPath, 
        paramFormat, 
        summaryFile);

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
