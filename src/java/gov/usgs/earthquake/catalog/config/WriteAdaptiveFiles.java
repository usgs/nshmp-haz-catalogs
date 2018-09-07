package gov.usgs.earthquake.catalog.config;

import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import gov.usgs.earthquake.catalog.AgridModel;
import gov.usgs.earthquake.catalog.AgridUtils;
import gov.usgs.earthquake.catalog.config.AdaptiveConfig.AdaptiveSmoothing;
import gov.usgs.earthquake.catalog.config.AdaptiveConfig.Completeness;
import gov.usgs.earthquake.catalog.config.AdaptiveConfig.Smoothing;
import gov.usgs.earthquake.catalog.config.CatalogConfig.SourceBoundaries;

public class WriteAdaptiveFiles {
  static final String FILE_NAME = "agridParameters.in";
  
  private WriteAdaptiveFiles() {}
  
  public static Path writeParameterFile(
      AgridModel agridModel,
      Path adaptiveDirPath,
      String parameterFileName) throws IOException {
    AgridConfig config = agridModel.config;
    
    Path out = adaptiveDirPath.resolve(parameterFileName);
    
    BufferedWriter writer = Files.newBufferedWriter(out, StandardCharsets.UTF_8); 
    
    /* Write adaptive smoothing parameters */
    AdaptiveSmoothing adaptiveSmoothing = config.adaptive.adaptiveSmoothing;
    writeAdaptiveSmoothing(writer, adaptiveSmoothing);

    /* Write catalog parameters */
    writeCatalog(writer, config, agridModel.catalogFilePath);
    
    /* Write Completeness levels */
    Completeness completeness = config.adaptive.completeness;
    writeCompleteness(writer, completeness);

    /* Write smoothing parameters */
    Smoothing smoothing = config.adaptive.smoothing;
    writeSmoothing(writer, smoothing);
    
    /* Write source boundaries*/
    SourceBoundaries bounds = config.catalog.sourceBoundaries; 
    writeSourceBoundaries(writer, bounds);
    
    writer.write("Output_Name=adaptiveAgrid");
    writer.close();
    
    return out;
  }
  
  private static void writeAdaptiveSmoothing(
      BufferedWriter writer, 
      AdaptiveSmoothing adaptiveSmoothing) throws IOException {
    writer.write("# Adaptive smoothing parameters \n");
    
    writer.write(String.format(
        "Adaptive_Neighbor_Number=%d\n", 
        adaptiveSmoothing.adaptiveNeighborNumber));
    
    writer.write(String.format(
        "Adaptive_Neighbor_Number2=%d\n", 
        adaptiveSmoothing.adaptiveNeighborNumber2));
    
    writer.write(String.format(
        "logical_Adaptive_smoothing=%d\n", 
        AgridUtils.boolean2Int(adaptiveSmoothing.applyAdaptiveSmoothing)));
    
    writer.write(String.format(
        "logical_Avg_Neighbor=%d\n", 
        AgridUtils.boolean2Int(adaptiveSmoothing.useAverageNeighbor)));
    
    writer.write(String.format("Sigma_Max=%.4f\n", adaptiveSmoothing.maxSigma));
    
    writer.write(String.format("Sigma_Min=%.4f\n", adaptiveSmoothing.minSigma));
  }
  
  private static void writeCatalog(
      BufferedWriter writer, 
      AgridConfig config,
      Path catalogFilePath) throws IOException {
    CatalogConfig catalog = config.catalog;
    
    writer.write("# Catalog parameters \n");
    writer.write(String.format("b_Value=%.4f\n", catalog.bValue));
    writer.write(String.format("Catalog_Name=%s\n", catalogFilePath));
    writer.write(String.format("End_yr_Catalog=%d\n", catalog.catalogEndYear));
    writer.write(String.format("Min_Magnitude=%.4f\n", config.adaptive.completeness.minMagnitude)); 
  }
  
  private static void writeCompleteness(BufferedWriter writer, Completeness completeness) 
      throws IOException {
    writer.write("# Completeness levels \n");
    
    writer.write("N_Completeness_Levels=1\n");
    
    writer.write(String.format(
        "Variable_Completeness=%d\n", 
        AgridUtils.boolean2Int(completeness.applyVariableCompleteness)));
    
    writer.write(String.format("Completeness_File=%s\n", completeness.completenessFile));
    
    writer.write(String.format(
        "Catalog_Gaps=%d\n", 
        AgridUtils.boolean2Int(completeness.applyCatalogGaps)));
    
    writer.write(String.format(
        "Polygons_for_Gaps_Path=%s\n",
        completeness.catalogGapsPolygonPath));
    
    writer.write(String.format(
        "Catalog_Gaps_File=%s\n", 
        completeness.catalogGapsFile));
    
    writer.write(String.format("Completeness_Level_yr_1=%d\n", completeness.catalogStartYear));
    writer.write(String.format("Completeness_Level_M_1=%.4f\n", completeness.minMagnitude));
    
    writer.write(String.format("dMag_value=%.4f\n", completeness.magnitudeIncrement));

    writer.write(String.format(
        "Correct_Area_for_Latitude=%d\n", 
        AgridUtils.boolean2Int(completeness.useCorrectLatitudeArea)));
  }
  
  private static void writeSmoothing(BufferedWriter writer, Smoothing smoothing) 
      throws IOException {
    writer.write(String.format("# Smoothing parameters \n"));
    writer.write(String.format("logical_Cumulative_rates=%d\n", AgridUtils.boolean2Int(smoothing.applyCumulativeRates)));
    writer.write(String.format("logical_Adjust_completeness=%d\n", AgridUtils.boolean2Int(smoothing.adjustCompleteness)));
    writer.write(String.format("logical_Effective_Num_Eqs=%d\n", AgridUtils.boolean2Int(smoothing.applyEffectiveNumberEquations)));
    writer.write(String.format("logical_Apply_smoothing=%d\n", AgridUtils.boolean2Int(smoothing.applySmoothing)));
    writer.write(String.format("Sigma_Fix=%.4f\n", smoothing.fixedSigma));
    writer.write(String.format("Gauss_PowerLaw_Kernel=%s\n", smoothing.gaussPowerLawKernel));
  }
  
  private static void writeSourceBoundaries(BufferedWriter writer, SourceBoundaries bounds) 
      throws IOException {
    writer.write("# Source boundaries \n");
    writer.write(String.format("Min_Lat=%.4f\n", bounds.minLatitude));
    writer.write(String.format("Max_Lat=%.4f\n", bounds.maxLatitude));
    writer.write(String.format("Min_Lon=%.4f\n", bounds.minLongitude));
    writer.write(String.format("Max_Lon=%.4f\n", bounds.maxLongitude));
    writer.write(String.format("Inc_Lat=%.3f\n", bounds.latitudeIncrement));
    writer.write(String.format("Inc_Lon=%.3f\n", bounds.longitudeIncrement));
  }
  
}
