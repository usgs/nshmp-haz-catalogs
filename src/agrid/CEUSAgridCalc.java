package agrid;

import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;

import agrid.config.AgridConfig;

public class CEUSAgridCalc {
  static final Path CLASS_PATH;
  public static final Path ETC_PATH;
  static final Path ADAPTIVE_EXE_PATH;
  static final Path FIXED_EXE_PATH;
  
  static {
    CLASS_PATH = Paths.get(System.getProperty("java.class.path"))
        .getParent()
        .normalize();
    
    ETC_PATH = CLASS_PATH.resolve("../../etc/").toAbsolutePath().normalize();
    AgridUtils.checkPath(ETC_PATH);
    
    ADAPTIVE_EXE_PATH = CLASS_PATH.resolve("../exe/adaptiveAgrid").toAbsolutePath().normalize();
    AgridUtils.checkPath(ADAPTIVE_EXE_PATH);
    
    FIXED_EXE_PATH = CLASS_PATH.resolve("../exe/fixedAgrid").toAbsolutePath().normalize();
    AgridUtils.checkPath(FIXED_EXE_PATH);
  }
  
  public static void main(String[] args) throws IOException, InterruptedException {
    AgridArguments agridArgs = AgridArguments.getArguments(args);
    AgridConfig config = agridArgs.config;
    Path catalogFilePath = agridArgs.catalogFilePath;
    
    CEUSAdaptiveCalc.runAdaptive(config, catalogFilePath);
    CEUSFixedCalc.runFixed(config, catalogFilePath);

    Path out = config.output.directory.resolve("config.json"); 
    config.write(out);
    System.out.printf("\n\nFiles reside in [%s] \n\n", config.output.directory);
  }
  
  static class AgridArguments {
    AgridConfig config;
    Path catalogFilePath;
    
    private AgridArguments(AgridConfig config, Path catalogFilePath) {
      this.catalogFilePath = catalogFilePath;
      this.config = config;
    }
    
    static AgridArguments getArguments(String args[]) throws IOException {
      int argCount = args.length;
     
      Path catalogFilePath = Paths.get(args[0]).toAbsolutePath().normalize();
      AgridUtils.checkPath(catalogFilePath);
      
      AgridConfig config = AgridConfig.withDefaults();
      
      if (argCount == 2) {
        Path userConfigPath = Paths.get(args[1]);
        AgridUtils.checkPath(userConfigPath);
        
        config = AgridConfig.builder()
            .copyOf(config)
            .extend(AgridConfig.builder().fromFile(userConfigPath))
            .build();
      }
      
      config.output.createOutputDirectory();
      
      return new AgridArguments(config, catalogFilePath);
    }

  }
  
}
