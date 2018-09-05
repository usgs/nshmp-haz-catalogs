package gov.usgs.earthquake.catalog.config;

import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import gov.usgs.earthquake.catalog.AgridModel.AgridProperties;
import gov.usgs.earthquake.catalog.AgridUtils;

public class AdaptiveConfig {
  public final AdaptiveSmoothing adaptiveSmoothing;
  public final Completeness completeness;
  public final Smoothing smoothing;
  
  private AdaptiveConfig(Builder builder) {
    this.adaptiveSmoothing = builder.adaptiveSmoothing.build();
    this.completeness = builder.completeness.build();
    this.smoothing = builder.smoothing.build();
  }
  
  public static void writeAdaptiveConfig(AgridConfig config, Path out) throws IOException {
    AdaptiveConfigOutput adaptiveConfig = new AdaptiveConfigOutput(config);
    
    BufferedWriter writer = Files.newBufferedWriter(
        out, 
        StandardCharsets.UTF_8);
  
    AgridConfig.GSON.toJson(adaptiveConfig, writer);
    writer.close();
  }
 
  public static Builder builder() {
    return new Builder();
  }

  public static class Builder {
    private AdaptiveSmoothing.Builder adaptiveSmoothing;
    private Completeness.Builder completeness;
    private Smoothing.Builder smoothing;
    
    private Builder() {
      adaptiveSmoothing = AdaptiveSmoothing.builder();
      completeness = Completeness.builder();
      smoothing = Smoothing.builder();
    }
    
    public AdaptiveConfig build() {
      return new AdaptiveConfig(this);
    }
    
    public Builder copy(AdaptiveConfig adaptive) {
      this.adaptiveSmoothing.copy(adaptive.adaptiveSmoothing);
      this.completeness.copy(adaptive.completeness);
      this.smoothing.copy(adaptive.smoothing);
      return this;
    }
    
    public Builder extend(final Builder that) {
      this.adaptiveSmoothing.extend(that.adaptiveSmoothing);
      this.completeness.extend(that.completeness);
      this.smoothing.extend(that.smoothing);
      return this;
    }
    
    public Builder adaptiveSmoothing(AdaptiveSmoothing adaptiveSmoothing) {
      this.adaptiveSmoothing.copy(adaptiveSmoothing); 
      return this;
    }
    
    public Builder completeness(Completeness completeness) {
      this.completeness.copy(completeness);
      return this;
    }
    
    public Builder smoothing(Smoothing smoothing) {
      this.smoothing.copy(smoothing);
      return this;
    }
    
    public static Builder withDefaults(AgridProperties agridProperties) {
      Builder builder = new Builder();
      builder.adaptiveSmoothing = AdaptiveSmoothing.Builder.withDefaults();
      builder.completeness = Completeness.Builder.withDefaults(agridProperties);
      builder.smoothing = Smoothing.Builder.withDefaults();
      return builder;
    }
    
  }
  
  public static class AdaptiveSmoothing {
    public final Integer adaptiveNeighborNumber;
    public final Integer adaptiveNeighborNumber2;
    public final Boolean applyAdaptiveSmoothing;
    public final Double maxSigma;
    public final Double minSigma;
    public final Boolean useAverageNeighbor;
    
    private AdaptiveSmoothing(Builder builder) {
      adaptiveNeighborNumber = builder.adaptiveNeighborNumber;
      adaptiveNeighborNumber2 = builder.adaptiveNeighborNumber2;
      applyAdaptiveSmoothing = builder.applyAdaptiveSmoothing;
      maxSigma = builder.maxSigma;
      minSigma = builder.minSigma;
      useAverageNeighbor = builder.useAverageNeighbor;
    }
    
    static Builder builder() {
      return new Builder();
    }
    
    static class Builder {
      private  Integer adaptiveNeighborNumber;
      private  Integer adaptiveNeighborNumber2;
      private  Boolean applyAdaptiveSmoothing;
      private  Double maxSigma;
      private  Double minSigma;
      private  Boolean useAverageNeighbor;
    
      AdaptiveSmoothing build() {
        return new AdaptiveSmoothing(this);
      }
      
      Builder copy(AdaptiveSmoothing that) {
        this.adaptiveNeighborNumber = that.adaptiveNeighborNumber;
        this.adaptiveNeighborNumber2 = that.adaptiveNeighborNumber2;
        this.applyAdaptiveSmoothing = that.applyAdaptiveSmoothing;
        this.maxSigma = that.maxSigma;
        this.minSigma = that.minSigma;
        this.useAverageNeighbor = that.useAverageNeighbor;
        return this;
      }
      
      Builder extend(Builder that) {
        if (that.adaptiveNeighborNumber != null) {
          this.adaptiveNeighborNumber = that.adaptiveNeighborNumber;
        }
        
        if (that.adaptiveNeighborNumber2 != null) {
          this.adaptiveNeighborNumber2 = that.adaptiveNeighborNumber2;
        }
        
        if (that.applyAdaptiveSmoothing != null) {
          this.applyAdaptiveSmoothing = that.applyAdaptiveSmoothing;
        }
        
        if (that.maxSigma != null) {
          this.maxSigma = that.maxSigma;
        }
        
        if (that.minSigma != null) {
          this.minSigma = that.minSigma;
        }
        
        if (that.useAverageNeighbor != null) {
          this.useAverageNeighbor = that.useAverageNeighbor;
        }
        
        return this;
      }
      
      static Builder withDefaults() {
        Builder builder = new Builder();
        builder.adaptiveNeighborNumber = 4;
        builder.adaptiveNeighborNumber2 = null;
        builder.applyAdaptiveSmoothing = true;
        builder.maxSigma = 200.0;
        builder.minSigma = 3.0;
        builder.useAverageNeighbor = false;
        return builder;
      }
    }
  }
  
  public static class Completeness {
    public final Boolean applyCatalogGaps;
    public final Boolean applyVariableCompleteness;
    public final Path catalogGapsFile;
    public final Path catalogGapsPolygonPath;
    public final Integer catalogStartYear;
    public final Path completenessFile;
    public final Double minMagnitude;
    public final Double magnitudeIncrement;
    public final Boolean useCorrectLatitudeArea;
    
    private Completeness(Builder builder) {
      this.applyCatalogGaps = builder.applyCatalogGaps;
      this.applyVariableCompleteness = builder.applyVariableCompleteness;
      this.catalogGapsFile = builder.catalogGapsFile;
      this.catalogGapsPolygonPath = builder.catalogGapsPolygonPath;
      this.catalogStartYear = builder.catalogStartYear;
      this.completenessFile = builder.completenessFile;
      this.magnitudeIncrement = builder.magnitudeIncrement;
      this.minMagnitude = builder.minMagnitude;
      this.useCorrectLatitudeArea = builder.useCorrectLatitudeArea;
    }
    
    public static Builder builder() {
      return new Builder();
    }
    
    public static class Builder {
      private Boolean applyCatalogGaps;
      private Boolean applyVariableCompleteness;
      private Path catalogGapsFile;
      private Path catalogGapsPolygonPath;
      private Integer catalogStartYear;
      private Path completenessFile;
      private Double minMagnitude;
      private Double magnitudeIncrement;
      private Boolean useCorrectLatitudeArea;
      
      public Completeness build() {
        AgridUtils.checkPath(catalogGapsFile);
        AgridUtils.checkPath(catalogGapsPolygonPath);
        AgridUtils.checkPath(completenessFile);
        return new Completeness(this);
      }
     
      public Builder copy(Completeness that) {
        this.applyCatalogGaps = that.applyCatalogGaps;
        this.applyVariableCompleteness = that.applyVariableCompleteness;
        this.catalogGapsFile = that.catalogGapsFile;
        this.catalogGapsPolygonPath = that.catalogGapsPolygonPath;
        this.catalogStartYear = that.catalogStartYear;
        this.completenessFile = that.completenessFile;
        this.magnitudeIncrement = that.magnitudeIncrement;
        this.minMagnitude = that.minMagnitude;
        this.useCorrectLatitudeArea = that.useCorrectLatitudeArea;
        return this;
      }
      
      Builder extend(final Builder that) {
        if (that.applyCatalogGaps != null) {
          this.applyCatalogGaps = that.applyCatalogGaps;
        }
        
        if (that.applyVariableCompleteness != null) {
          this.applyVariableCompleteness = that.applyVariableCompleteness;
        }
        
        if (that.catalogGapsFile != null) {
          this.catalogGapsFile = that.catalogGapsFile;
        }
        
        if (that.catalogGapsPolygonPath != null) {
          this.catalogGapsPolygonPath = that.catalogGapsPolygonPath;
        }
        
        if (that.catalogStartYear != null) {
          this.catalogStartYear = that.catalogStartYear;
        }
        
        if (that.completenessFile != null) {
          this.completenessFile = that.completenessFile;
        }
        
        if (that.magnitudeIncrement != null) {
          this.magnitudeIncrement = that.magnitudeIncrement;
        }
        
        if (that.minMagnitude != null) {
          this.minMagnitude = that.minMagnitude;
        }
        
        if (that.useCorrectLatitudeArea != null) {
          this.useCorrectLatitudeArea = that.useCorrectLatitudeArea;
        }
        
        return this;
      }
      
      static Builder withDefaults(AgridProperties agridProperties) {
        Builder builder = new Builder();
        builder.applyCatalogGaps = true;
        builder.applyVariableCompleteness = true;
        builder.catalogGapsFile = agridProperties.adaptiveEtcDirPath 
            .resolve("catalog_gaps.txt")
            .toAbsolutePath()
            .normalize();
        builder.catalogGapsPolygonPath = agridProperties.adaptiveEtcDirPath
            .resolve("polygons_for_gaps")
            .toAbsolutePath()
            .normalize();
        builder.catalogStartYear = 1870;
        builder.completenessFile = agridProperties.adaptiveEtcDirPath
            .resolve("completeness_maps/compl_CEUS_M3p2.txt")
            .toAbsolutePath()
            .normalize();
        builder.magnitudeIncrement = 0.1;
        builder.minMagnitude = 3.2;
        builder.useCorrectLatitudeArea = true;
        return builder;
      }
    }
  }
  
  public static class Smoothing {
    public final Boolean adjustCompleteness;
    public final Boolean applyCumulativeRates;
    public final Boolean applyEffectiveNumberEquations;
    public final Boolean applySmoothing;
    public final Boolean checkSmoothingParameters;
    public final Double fixedSigma;
    public final String gaussPowerLawKernel;
    
    private Smoothing(Builder builder) {
      this.adjustCompleteness = builder.adjustCompleteness;
      this.applyCumulativeRates = builder.applyCumulativeRates;
      this.applyEffectiveNumberEquations = builder.applyEffectiveNumberEquations;
      this.applySmoothing = builder.applySmoothing;
      this.checkSmoothingParameters = builder.checkSmoothingParameters;
      this.fixedSigma = builder.fixedSigma;
      this.gaussPowerLawKernel = builder.gaussPowerLawKernel;
    }
    
    static Builder builder() {
      return new Builder();
    }
    
    static class Builder {
      private Boolean adjustCompleteness;
      private Boolean applyCumulativeRates;
      private Boolean applyEffectiveNumberEquations;
      private Boolean applySmoothing;
      private Boolean checkSmoothingParameters;
      private Double fixedSigma;
      private String gaussPowerLawKernel;
      
      private Builder() {}
     
      Smoothing build() {
        return new Smoothing(this);
      }
      
      Builder copy(Smoothing that) {
        this.adjustCompleteness = that.adjustCompleteness;
        this.applyCumulativeRates = that.applyCumulativeRates;
        this.applyEffectiveNumberEquations = that.applyEffectiveNumberEquations;
        this.applySmoothing = that.applySmoothing;
        this.checkSmoothingParameters = that.checkSmoothingParameters;
        this.fixedSigma = that.fixedSigma;
        this.gaussPowerLawKernel = that.gaussPowerLawKernel;
        return this;
      }
      
      Builder extend(final Builder that) {
        if (that.adjustCompleteness != null) {
          this.adjustCompleteness = that.adjustCompleteness;
        }
        
        if (that.applyCumulativeRates != null) {
          this.applyCumulativeRates = that.applyCumulativeRates;
        }
        
        if (that.applyEffectiveNumberEquations != null) {
          this.applyEffectiveNumberEquations = that.applyEffectiveNumberEquations;
        }
        
        if (that.applySmoothing != null) {
          this.applySmoothing = that.applySmoothing;
        }
        
        if (that.checkSmoothingParameters != null) {
          this.checkSmoothingParameters = that.checkSmoothingParameters;
        }
        
        if (that.fixedSigma != null) {
          this.fixedSigma = that.fixedSigma;
        }
        
        if (that.gaussPowerLawKernel != null) {
          this.gaussPowerLawKernel = that.gaussPowerLawKernel;
        }
        
        return this;
      }
      
      static Builder withDefaults() {
        Builder builder = new Builder();
        builder.adjustCompleteness = false;
        builder.applyCumulativeRates = false;
        builder.applyEffectiveNumberEquations = false;
        builder.applySmoothing = true;
        builder.checkSmoothingParameters = false;
        builder.fixedSigma = null;
        builder.gaussPowerLawKernel = "gauss";
        return builder;
      }
    }
  }
  
  private static class AdaptiveConfigOutput {
    private final CatalogConfig catalog;
    private final AdaptiveConfig adaptive;
    private final OutputConfig output;
    
    private AdaptiveConfigOutput(final AgridConfig config) {
      catalog = config.catalog;
      adaptive = config.adaptive;
      output = config.output;
    }
    
  }
  
}
