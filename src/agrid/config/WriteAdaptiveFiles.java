package agrid.config;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import agrid.AgridUtils;
import agrid.config.AdaptiveConfig.AdaptiveSmoothing;
import agrid.config.AdaptiveConfig.Completeness;
import agrid.config.AdaptiveConfig.Smoothing;
import agrid.config.CatalogConfig.SourceBoundaries;

public class WriteAdaptiveFiles {
  static final String FILE_NAME = "smoothingParameters.in";
  
  private WriteAdaptiveFiles() {}
  
  public static Path writeCatalogGapsFile(
      AgridConfig config,
      Path adaptiveDirectoryPath) throws IOException {
    AdaptiveConfig adaptive = config.adaptive;
    Path catalogGapsInputPath = adaptive.completeness.catalogGapsFile;
    
    String catalogGapsDir = catalogGapsInputPath.getParent().toAbsolutePath().normalize().toString();
    Path catalogGapsOutputPath = Paths.get(
        adaptiveDirectoryPath.toString(), 
        catalogGapsInputPath.getFileName().toString())
        .toAbsolutePath()
        .normalize();
    
    BufferedWriter writer = Files.newBufferedWriter(catalogGapsOutputPath, StandardCharsets.UTF_8);
    BufferedReader reader = Files.newBufferedReader(catalogGapsInputPath, StandardCharsets.UTF_8);

    String line = null;
    while((line = reader.readLine()) != null) {
      line = catalogGapsDir + "/" + line + "\n";
      writer.write(line);
    }
    
    writer.close();
    reader.close();
    
    return catalogGapsOutputPath;
  }
  
  public static Path writeParameterFile(
      AgridConfig config,
      Path catalogFilePath,
      Path adaptiveDirectoryPath) throws IOException {
    Path out = adaptiveDirectoryPath.resolve(FILE_NAME);
    
    BufferedWriter writer = Files.newBufferedWriter(out, StandardCharsets.UTF_8); 
    
    /* Write adaptive smoothing parameters */
    AdaptiveSmoothing adaptiveSmoothing = config.adaptive.adaptiveSmoothing;
    writeAdaptiveSmoothing(writer, adaptiveSmoothing);

    /* Write catalog parameters */
    writeCatalog(writer, config, catalogFilePath);
    
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
    
    writer.write(String.format("Sigma_Max=%f\n", adaptiveSmoothing.maxSigma));
    
    writer.write(String.format("Sigma_Min=%f\n", adaptiveSmoothing.minSigma));
  }
  
  private static void writeCatalog(
      BufferedWriter writer, 
      AgridConfig config,
      Path catalogFilePath) throws IOException {
    CatalogConfig catalog = config.catalog;
    
    writer.write("#Catalog parameters \n");
    writer.write(String.format("b_Value=%f\n", catalog.bValue));
    writer.write(String.format("Catalog_Name=%s\n", catalogFilePath));
    writer.write(String.format("End_yr_Catalog=%d\n", catalog.catalogEndYear));
    writer.write(String.format("Min_Magnitude=%f\n", config.adaptive.completeness.minMagnitude)); 
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
        "Catalog_Gaps_File=%s\n", 
        completeness.catalogGapsFile.getFileName()));
    
    writer.write(String.format("Completeness_Level_yr_1=%d\n", completeness.catalogStartYear));
    writer.write(String.format("Completeness_Level_M_1=%f\n", completeness.minMagnitude));
    
    writer.write(String.format("dMag_value=%f\n", completeness.magnitudeIncrement));

    writer.write(String.format(
        "Correct_Area_for_Latitude=%d\n", 
        AgridUtils.boolean2Int(completeness.useCorrectLatitudeArea)));
  }
  
  private static void writeSmoothing(BufferedWriter writer, Smoothing smoothing) 
      throws IOException {
    writer.write(String.format("# smoothing parameters \n"));
    writer.write(String.format("logical_Cumulative_rates=%d\n", AgridUtils.boolean2Int(smoothing.applyCumulativeRates)));
    writer.write(String.format("logical_Adjust_completeness=%d\n", AgridUtils.boolean2Int(smoothing.adjustCompleteness)));
    writer.write(String.format("logical_Effective_Num_Eqs=%d\n", AgridUtils.boolean2Int(smoothing.applyEffectiveNumberEquations)));
    writer.write(String.format("logical_Apply_smoothing=%d\n", AgridUtils.boolean2Int(smoothing.applySmoothing)));
    writer.write(String.format("Sigma_Fix=%f\n", smoothing.fixedSigma));
    writer.write(String.format("Gauss_PowerLaw_Kernel=%s\n", smoothing.gaussPowerLawKernel));
  }
  
  private static void writeSourceBoundaries(BufferedWriter writer, SourceBoundaries bounds) 
      throws IOException {
    writer.write("# source boundaries \n");
    writer.write(String.format("Min_Lat=%f\n", bounds.minLatitude));
    writer.write(String.format("Max_Lat=%f\n", bounds.maxLatitude));
    writer.write(String.format("Min_Lon=%f\n", bounds.minLongitude));
    writer.write(String.format("Max_Lon=%f\n", bounds.maxLongitude));
    writer.write(String.format("Inc_Lat=%f\n", bounds.latitudeIncrement));
    writer.write(String.format("Inc_Lon=%f\n", bounds.longitudeIncrement));
  }
  
}
