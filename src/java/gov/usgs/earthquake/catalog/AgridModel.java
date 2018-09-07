package gov.usgs.earthquake.catalog;

import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Properties;

import gov.usgs.earthquake.catalog.config.AgridConfig;

public class AgridModel {
  public final Path adaptiveExePath;
  public final Path catalogFilePath;
  public final AgridConfig config;
  public final Path adaptiveEtcDirPath;
  public final Path fixedExePath;
  
  private AgridModel(AgridArguments agridArguments, AgridProperties agridProperties) {
    adaptiveExePath = agridProperties.adaptiveExePath;
    catalogFilePath = agridArguments.catalogFilePath;
    config = agridArguments.config;
    adaptiveEtcDirPath = agridProperties.adaptiveEtcDirPath;
    fixedExePath = agridProperties.fixedExePath;
  }
  
  public static AgridModel getAgridModel(String[] args) throws IOException {
    AgridProperties agridProperties = AgridProperties.getProperties();
    AgridArguments agridArguments = AgridArguments.getArguments(args, agridProperties);
    return new AgridModel(agridArguments, agridProperties);
  }
  
  public static class AgridProperties {
    public final Path adaptiveExePath;
    public final Path adaptiveEtcDirPath;
    public final Path fixedExePath;
    
    private AgridProperties(Path adaptiveExePath, Path adaptiveEtcDirPath, Path fixedExePath) {
      this.adaptiveExePath = adaptiveExePath;
      this.adaptiveEtcDirPath = adaptiveEtcDirPath;
      this.fixedExePath = fixedExePath;
    }
    
    static AgridProperties getProperties() throws IOException {
      InputStream input = CEUSAgridCalc.class.getResourceAsStream("config.properties");
      Properties props = new Properties();
      props.load(input);
      
      Path adaptiveExePath = Paths.get(props.getProperty("adaptiveExe"));
      AgridUtils.checkPath(adaptiveExePath);
      Path adaptiveEtcDirPath = Paths.get(props.getProperty("adaptiveEtcDir"));
      AgridUtils.checkPath(adaptiveEtcDirPath);
      Path fixedExePath = Paths.get(props.getProperty("fixedExe"));
      AgridUtils.checkPath(fixedExePath);

      return new AgridProperties(adaptiveExePath, adaptiveEtcDirPath, fixedExePath);
    }

  }
  
  public static class AgridArguments {
    public final AgridConfig config;
    public Path catalogFilePath;
    
    private AgridArguments(AgridConfig config, Path catalogFilePath) {
      this.catalogFilePath = catalogFilePath;
      this.config = config;
    }
    
    static AgridArguments getArguments(
        String args[],
        AgridProperties agridProperties) throws IOException {
      int argCount = args.length;
     
      Path catalogFilePath = Paths.get(args[0]).toAbsolutePath().normalize();
      AgridUtils.checkPath(catalogFilePath);
      
      AgridConfig config = AgridConfig.withDefaults(agridProperties);
      
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
