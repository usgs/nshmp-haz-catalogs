package agrid;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import agrid.config.AgridConfig;

public class AgridUtils {
  
  public static Path checkPath(Path path) {
    if (!Files.exists(path)) {
      String errMsg = String.format("[%s] does not exist", path);
      throw new RuntimeException(errMsg);
    }

    return path;
  }
  
  public static int boolean2Int(Boolean bool) {
    return bool ? 1 : 0;
  }

  public static Path createDirectory(Path dir) throws IOException {
    int i = 1;
    Path incrementedDir = dir;
    while (Files.exists(incrementedDir)) {
      incrementedDir = incrementedDir.resolveSibling(dir.getFileName() + "-" + i);
      i++;
    }
    Files.createDirectories(incrementedDir);
    return incrementedDir.toAbsolutePath().normalize();
  }
  
  static AgridConfig getCalcArguments(String[] args) throws IOException {
    int argCount = args.length;
   
    Path catalogPath = Paths.get(args[0]).toAbsolutePath().normalize();
    AgridConfig config = AgridConfig.withDefaults();
    
    if (argCount == 2) {
      Path userConfigPath = Paths.get(args[1]);
      
      config = AgridConfig.builder()
          .copyOf(config)
          .extend(AgridConfig.builder().fromFile(userConfigPath))
          .build();
    }
    
    return config;
  }
  
}
