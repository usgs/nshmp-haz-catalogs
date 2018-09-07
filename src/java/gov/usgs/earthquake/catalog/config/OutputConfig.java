package gov.usgs.earthquake.catalog.config;

import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;

import gov.usgs.earthquake.catalog.AgridUtils;

public class OutputConfig {
  public final String adaptiveFile;
  public Path directory;
  
  private OutputConfig(Builder builder) {
    directory = builder.directory;
    adaptiveFile = builder.adaptiveFile;
  }
  
  public void createOutputDirectory() throws IOException {
    directory = AgridUtils.createDirectory(directory);
  }
  
  public static Builder builder() {
    return new Builder();
  }
  
  public static class Builder {
    private String adaptiveFile;
    private Path directory;
  
    private Builder() {}
   
    public OutputConfig build() {
      return new OutputConfig(this);
    }
    
    public Builder copy(OutputConfig that) {
      this.adaptiveFile = that.adaptiveFile;
      this.directory = that.directory;
      return this;
    }
    
    public Builder extend(final Builder that) {
      if (that.adaptiveFile != null) {
        this.adaptiveFile = that.adaptiveFile;
      }
      
      if (that.directory != null) {
        this.directory = that.directory;
      }
      
      return this;
    }
    
    public static Builder withDefaults() {
      Builder builder = new Builder();
      builder.adaptiveFile = "adaptiveAgrid";
      builder.directory = Paths.get("agridOut");
      return builder;
    }
    
  }
  
}
