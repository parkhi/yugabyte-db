// Copyright (c) YugaByte, Inc.

package com.yugabyte.yw.forms;

import io.swagger.annotations.ApiModel;
import io.swagger.annotations.ApiModelProperty;
import javax.validation.constraints.Min;
import lombok.Data;
import lombok.experimental.Accessors;

/** Universe perf advisor settings. Fields are nullable - which means use default global value */
@ApiModel
@Data
@Accessors(chain = true)
public class PerfAdvisorSettingsFormData {

  @ApiModelProperty(value = "Enable/disable perf advisor runs for the universe")
  private Boolean enabled;

  @Min(5)
  @ApiModelProperty(value = "Perf advisor runs frequency, in minutes")
  private Integer runFrequencyMins;

  @ApiModelProperty(value = "Perf advisor connection skew threshold")
  @Min(1)
  private Double connectionSkewThreshold;

  @ApiModelProperty(value = "Perf advisor connection skew min connections")
  @Min(1)
  private Integer connectionSkewMinConnections;

  @ApiModelProperty(value = "Perf advisor connection skew check interval")
  @Min(1)
  private Integer connectionSkewIntervalMins;

  @ApiModelProperty(value = "Perf advisor cpu skew threshold")
  @Min(1)
  private Double cpuSkewThreshold;

  @ApiModelProperty(value = "Perf advisor cpu skew min cpu usage")
  @Min(1)
  private Double cpuSkewMinUsage;

  @ApiModelProperty(value = "Perf advisor cpu skew check interval")
  @Min(1)
  private Integer cpuSkewIntervalMins;

  @ApiModelProperty(value = "Perf advisor CPU usage threshold")
  @Min(1)
  private Double cpuUsageThreshold;

  @ApiModelProperty(value = "Perf advisor CPU usage check interval")
  @Min(1)
  private Integer cpuUsageIntervalMins;

  @ApiModelProperty(value = "Perf advisor query skew threshold")
  @Min(1)
  private Double querySkewThreshold;

  @ApiModelProperty(value = "Perf advisor query skew min queries")
  @Min(1)
  private Integer querySkewMinQueries;

  @ApiModelProperty(value = "Perf advisor query skew check interval")
  @Min(1)
  private Integer querySkewIntervalMins;

  @ApiModelProperty(value = "Perf advisor rejected connections threshold")
  @Min(1)
  private Integer rejectedConnThreshold;

  @ApiModelProperty(value = "Perf advisor rejected connections check interval")
  @Min(1)
  private Integer rejectedConnIntervalMins;
}
