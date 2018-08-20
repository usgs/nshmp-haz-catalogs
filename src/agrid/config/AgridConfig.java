package agrid.config;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.Reader;
import java.lang.reflect.Type;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonDeserializationContext;
import com.google.gson.JsonDeserializer;
import com.google.gson.JsonElement;
import com.google.gson.JsonParseException;
import com.google.gson.JsonPrimitive;
import com.google.gson.JsonSerializationContext;
import com.google.gson.JsonSerializer;

public class AgridConfig {
  static final Gson GSON; 
  
  static {
    GSON = new GsonBuilder()
        .registerTypeHierarchyAdapter(Path.class, new PathConverter())
        .disableHtmlEscaping()
        .serializeNulls()
        .setPrettyPrinting()
        .create();
  }
  
  public final CatalogConfig catalog;
  public final AdaptiveConfig adaptive;
  public final FixedConfig fixed;
  public final OutputConfig output;
  
  private AgridConfig(Builder builder) {
    this.catalog = builder.catalog.build();
    this.adaptive = builder.adaptive.build();
    this.fixed = builder.fixed.build();
    this.output = builder.output.build();
  }

  public void write(Path out) throws IOException {
    BufferedWriter writer = Files.newBufferedWriter(
        out, 
        StandardCharsets.UTF_8);
  
    GSON.toJson(this, writer);
    writer.close();
  }
 
  public String toJsonString() {
    return GSON.toJson(this, AgridConfig.class);
  }
  
  public static AgridConfig withDefaults() {
    return Builder.withDefaults().build();
  }
  
  public static Builder builder() {
    return new Builder();
  }
  
  public static class Builder {
    private AdaptiveConfig.Builder adaptive;
    private CatalogConfig.Builder catalog;
    private FixedConfig.Builder fixed;
    private OutputConfig.Builder output;
    
    private Builder() {
      adaptive = AdaptiveConfig.builder();
      catalog = CatalogConfig.builder();
      fixed = FixedConfig.builder();
      output = OutputConfig.builder();
    }
   
    public AgridConfig build() {
      return new AgridConfig(this);
    }
    
    public Builder copyOf(AgridConfig config) {
      this.adaptive.copy(config.adaptive);
      this.catalog.copy(config.catalog);
      this.fixed.copy(config.fixed);
      this.output.copy(config.output);
      return this;
    }
    
    public Builder fromFile(Path path) throws IOException {
      Reader reader = Files.newBufferedReader(path, StandardCharsets.UTF_8);
      Builder builder = GSON.fromJson(reader, Builder.class); 
      reader.close();
      return builder;
    }
   
    public Builder extend(Builder that) {
      this.adaptive.extend(that.adaptive);
      this.catalog.extend(that.catalog);
      this.fixed.extend(that.fixed);
      this.output.extend(that.output);
      return this;
    }
    
    public Builder adaptive(AdaptiveConfig adaptive) {
      this.adaptive.copy(adaptive);
      return this;
    }
    
    public Builder catalog(CatalogConfig catalog) {
      this.catalog.copy(catalog);
      return this;
    }
    
    public Builder fixed(FixedConfig fixed) {
      this.fixed.copy(fixed);
      return this;
    }
    
    public Builder output(OutputConfig output) {
      this.output.copy(output);
      return this;
    }
    
    public static Builder withDefaults() {
      Builder builder = new Builder();
      builder.adaptive = AdaptiveConfig.Builder.withDefaults();
      builder.catalog = CatalogConfig.Builder.withDefaults();
      builder.fixed = FixedConfig.Builder.withDefaults();
      builder.output = OutputConfig.Builder.withDefaults();
      return builder;
    }
    
  }
  
  private static class PathConverter implements JsonSerializer<Path>, JsonDeserializer<Path> {

    @Override
    public Path deserialize(
        JsonElement json, 
        Type typeOfT, 
        JsonDeserializationContext context) throws JsonParseException {
      return Paths.get(json.getAsString()).toAbsolutePath().normalize();
    }

    @Override
    public JsonElement serialize(
        Path path, 
        Type typeOfSrc, 
        JsonSerializationContext context) {
      return new JsonPrimitive(path.toAbsolutePath().normalize().toString());
    }
    
  }

}
