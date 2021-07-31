import { ChangeDetectionStrategy, ChangeDetectorRef, Component, OnInit } from '@angular/core';
import { AnyLightSourceSettings, BrightnessProcessorSettings, DeviceSettings } from 'src/app/data/light-strip-settings';
import { LightStripService } from 'src/app/services/light-strip-service';

@Component({
  selector: 'app-light-strip',
  templateUrl: './light-strip.component.html',
  styleUrls: ['./light-strip.component.scss'],
  changeDetection: ChangeDetectionStrategy.OnPush
})
export class LightStripComponent implements OnInit {

  source: AnyLightSourceSettings;
  brightnessProcessor = new BrightnessProcessorSettings();
  device = new DeviceSettings();

  getType() {
    return this.source?.$type ?? "none";
  }

  constructor(
    private readonly changeDetector: ChangeDetectorRef,
    private readonly lightStripService : LightStripService
  ) { }

  async ngOnInit() {
    this.source = await this.lightStripService.getLightSourceSettings();
    this.brightnessProcessor = await this.lightStripService.getBrightnessSettings();
    this.device = await this.lightStripService.getDeviceSettings();
    this.changeDetector.detectChanges();
  }

  async onSourceChange(event: any) {
    await this.lightStripService.switchLightSource(event.target.value);
    this.source = await this.lightStripService.getLightSourceSettings();
    this.changeDetector.detectChanges();
  }

  onBrightnessChange(event: any) {    
    this.brightnessProcessor.brightness = parseFloat(event.target.value);

    let settings = new BrightnessProcessorSettings();
    settings.brightness = this.brightnessProcessor.brightness;
    this.lightStripService.setBrightnessSettings(settings);
  }

  onLightCountChange(event: any) {    
    this.device.lightCount = parseFloat(event.target.value);

    let settings = new DeviceSettings();
    settings.lightCount = this.device.lightCount;
    this.lightStripService.setDeviceSettings(settings);
  }
}
