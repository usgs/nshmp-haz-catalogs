package gov.usgs.earthquake.catalog.config;

import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

public class FixedConfig {
  public final CompletenessModel completenessModel;
  public final Double smoothingCorrelationLength;
  public final Boolean forceUniformEtennzNewmadRates;
 
  private FixedConfig(Builder builder) {
    completenessModel = builder.completenessModel.build();
    smoothingCorrelationLength = builder.smoothingCorrelationLength;
    forceUniformEtennzNewmadRates = builder.forceUniformEtennzNewmadRates;
  }
  
  public static void writeFixedConfig(AgridConfig config, Path out) throws IOException {
    FixedConfigOutput fixedConfig = new FixedConfigOutput(config);
    
    BufferedWriter writer = Files.newBufferedWriter(
        out, 
        StandardCharsets.UTF_8);
  
    AgridConfig.GSON.toJson(fixedConfig, writer);
    writer.close();
  }
 
  public static Builder builder() {
    return new Builder();
  }
  
  public static class Builder {
    private CompletenessModel.Builder completenessModel;
    private Double smoothingCorrelationLength;
    private Boolean forceUniformEtennzNewmadRates;
    
    private Builder() {
      completenessModel = CompletenessModel.builder();
    }
    
    public FixedConfig build() {
      return new FixedConfig(this);
    }
    
    public Builder copy(FixedConfig that) {
      this.completenessModel.copy(that.completenessModel);
      this.forceUniformEtennzNewmadRates = that.forceUniformEtennzNewmadRates;
      this.smoothingCorrelationLength = that.smoothingCorrelationLength;
      return this;
    }
    
    public Builder extend(Builder that) {
      this.completenessModel.extend(that.completenessModel);
      
      if (that.forceUniformEtennzNewmadRates != null) {
        this.forceUniformEtennzNewmadRates = that.forceUniformEtennzNewmadRates;
      }
      
      if (that.smoothingCorrelationLength != null) {
        this.smoothingCorrelationLength = that.smoothingCorrelationLength;
      }
      
      return this;
    }
    
    public static Builder withDefaults() {
      Builder builder = new Builder();
      builder.completenessModel = CompletenessModel.Builder.withDefaults();
      builder.forceUniformEtennzNewmadRates = true;
      builder.smoothingCorrelationLength = 50.0;
      return builder;
    }
    
  }
  
  public static class CompletenessModel {
    public final double[] magnitude;
    public final int[] year;
    public final String[] zone;
    
    private CompletenessModel(Builder builder) {
      magnitude = builder.magnitude;
      year = builder.year;
      zone = builder.zone;
    }
   
    public Path writeModelInput(Path fixedDirectoryPath) throws IOException {
      Path out = fixedDirectoryPath.resolve("model.in");
      BufferedWriter writer = Files.newBufferedWriter(out, StandardCharsets.UTF_8);
      
      int nZones = zone.length;
      for (int i = 0; i < nZones; i++) {
        writer.write(String.format("%f %d %s\n", magnitude[i], year[i], zone[i]));
      }
      
      writer.close();
      
      return out;
    }
    
    static Builder builder() {
      return new Builder();
    }
    
    static class Builder {
      private double[] magnitude;
      private int[] year;
      private String[] zone;
      
      private Builder() {}
      
      CompletenessModel build() {
        return new CompletenessModel(this);
      }
      
      Builder copy(CompletenessModel that) {
        this.magnitude = that.magnitude;
        this.year = that.year;
        this.zone = that.zone;
        return this;
      }
      
      Builder extend(final Builder that) {
        if (that.magnitude != null) {
          this.magnitude = that.magnitude;
        }
        
        if (that.year != null) {
          this.year = that.year;
        }
        
        if (that.zone != null) {
          this.zone = that.zone;
        }
        
        return this;
      }
      
      static Builder withDefaults() {
        Builder builder = new Builder();
        builder.magnitude = new double[] { 2.7, 2.7, 2.7, 2.7, 2.7, 2.7, 2.7 };
        builder.year = new int[] { 1970, 1930, 1975, 1980, 1980, 1960, 1980 };
        builder.zone = new String[] { "cz1", "cz2", "cz3", "cz4", "cz5", "cz6", "cz0" };
        return builder;
      }
    }
  }
  
  private static class FixedConfigOutput {
    private final CatalogConfig catalog;
    private final FixedConfig fixed;
    private final OutputConfig output;
    
    private FixedConfigOutput(final AgridConfig config) {
      catalog = config.catalog;
      fixed = config.fixed;
      output = config.output;
    }

  }
  
}
