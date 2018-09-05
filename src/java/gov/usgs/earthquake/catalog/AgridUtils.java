package gov.usgs.earthquake.catalog;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;

import com.google.common.base.Throwables;

public class AgridUtils {
  static final String GITHUB_URL = "https://github.com/usgs/nshmp-haz-catalogs";
  static final String JAVA_COMMAND = "java -cp nshmp-haz-catalogs.jar";
  
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
  
  static void checkArguments(String[] args, String javaUsageCommand) {
    int argCount = args.length;
    
    if (argCount < 1 || argCount > 2) {
      System.err.print(getUsage(javaUsageCommand));
      System.exit(1);
    }
  }
  
  static void handleError(Throwable e, String[] args, String javaUsageCommand) {
    String message = new StringBuilder()
        .append("\n")
        .append("ERROR: \n")
        .append("   Arguments: ").append(Arrays.toString(args)).append("\n\n")
        .append(Throwables.getStackTraceAsString(e))
        .append(getUsage(javaUsageCommand))
        .toString();
    
    System.err.print(message);
    System.exit(1);
  }
  
  static String getUsage(String javaUsageCommand) {
    return new StringBuilder()
      .append("\n")
      .append("Usage: \n")
      .append("   " + javaUsageCommand) 
      .append("\n\n")
      .append("Where: \n")
      .append("   'catalogFile' is the NSHMP earthquake catalog file \n")
      .append("   'config' (optional) supplies configuration for adaptive and fixed calculations") 
      .append("\n\n")
      .append("For more information, see: \n")
      .append("   " + GITHUB_URL)
      .append("\n\n")
      .toString();
  }
 
}
