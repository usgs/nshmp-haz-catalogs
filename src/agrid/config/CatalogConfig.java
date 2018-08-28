package agrid.config;

import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class CatalogConfig {
  public final Double bValue;
  public final Integer catalogEndYear;
  public final SourceBoundaries sourceBoundaries;
  
  private CatalogConfig(Builder builder) {
    bValue = builder.bValue;
    catalogEndYear = builder.catalogEndYear;
    sourceBoundaries = builder.sourceBoundaries.build();
  }

  static Builder builder() {
    return new Builder();
  }
  
  static class Builder {
    private Double bValue;
    private Integer catalogEndYear;
    private SourceBoundaries.Builder sourceBoundaries;
    
    private Builder() {
      sourceBoundaries = SourceBoundaries.builder();
    }
   
    CatalogConfig build() {
      return new CatalogConfig(this);
    }
    
    Builder copy(CatalogConfig that) {
      this.bValue = that.bValue;
      this.catalogEndYear = that.catalogEndYear;
      this.sourceBoundaries.copy(that.sourceBoundaries);
      return this;
    }
    
    Builder extend(final Builder that) {
      if (that.bValue != null) {
        this.bValue = that.bValue;
      }
      
      if (that.catalogEndYear != null) {
        this.catalogEndYear = that.catalogEndYear;
      }
      
      this.sourceBoundaries.extend(that.sourceBoundaries);
      
      return this;
    }
    
    static Builder withDefaults() {
      Builder builder = new Builder();
      builder.bValue = 1.0;
      builder.catalogEndYear = 2012;
      builder.sourceBoundaries = SourceBoundaries.Builder.withDefaults();
      return builder;
    }

  }
  
  public static class SourceBoundaries {
    public final Double latitudeIncrement;
    public final Double longitudeIncrement;
    public final Double maxLatitude;
    public final Double maxLongitude;
    public final Double minLatitude;
    public final Double minLongitude;
    
    private SourceBoundaries(Builder builder) {
      latitudeIncrement = builder.latitudeIncrement;
      longitudeIncrement = builder.longitudeIncrement;
      maxLatitude = builder.maxLatitude;
      maxLongitude = builder.maxLongitude;
      minLatitude = builder.minLatitude;
      minLongitude = builder.minLongitude;
    }

    public void writeFixedGridFile(Path fixedDirectoryPath) throws IOException {
      Path out = fixedDirectoryPath.resolve("ena.gctrl");
      
      BufferedWriter writer = Files.newBufferedWriter(out);
      
      writer.write(String.format("%6.4f %6.4f\n", minLatitude, maxLatitude));
      writer.write(String.format("%6.4f %6.4f\n", minLongitude, maxLongitude));
      writer.write(String.format("%3.3f %3.3f\n", latitudeIncrement, longitudeIncrement));
      writer.close();
    }
    
    static Builder builder() {
      return new Builder();
    }
    
    static class Builder {
      private Double latitudeIncrement;
      private Double longitudeIncrement;
      private Double maxLatitude;
      private Double maxLongitude;
      private Double minLatitude;
      private Double minLongitude;
      
      private Builder() {}
     
      SourceBoundaries build() {
        return new SourceBoundaries(this);
      }
      
      Builder copy(SourceBoundaries that) {
        this.latitudeIncrement = that.latitudeIncrement;
        this.longitudeIncrement = that.longitudeIncrement;
        this.maxLatitude = that.maxLatitude;
        this.maxLongitude = that.maxLongitude;
        this.minLatitude = that.minLatitude;
        this.minLongitude = that.minLongitude;
        return this;
      }
      
      Builder extend(final Builder that) {
        if (that.latitudeIncrement != null) {
          this.latitudeIncrement = that.latitudeIncrement;
        }
        
        if (that.longitudeIncrement != null) {
          this.longitudeIncrement = that.longitudeIncrement;
        }
        
        if (that.maxLatitude != null) {
          this.maxLatitude = that.maxLatitude;
        }
        
        if (that.maxLongitude != null) {
          this.maxLongitude = that.maxLongitude;
        }
        
        if (that.minLatitude != null) {
          this.minLatitude = that.minLatitude;
        }
        
        if (that.minLongitude != null) {
          this.minLongitude = that.minLongitude;
        }
        
        return this;
      }
      
      static Builder withDefaults() {
        Builder builder = new Builder();
        builder.latitudeIncrement = 0.1;
        builder.longitudeIncrement = 0.1;
        builder.maxLatitude = 53.0;
        builder.maxLongitude = -60.0;
        builder.minLatitude = 23.0;
        builder.minLongitude = -116.0;
        return builder;
      }

    }

  }

}
