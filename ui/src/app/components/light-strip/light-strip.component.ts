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

  areBrightnessDetailsVisible = false;

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

  onToggleBrightnessDetails(event: any) {
    this.areBrightnessDetailsVisible = !this.areBrightnessDetailsVisible;
  }

  onBrightnessChange(event: any) {    
    this.brightnessProcessor.brightness = parseFloat(event.target.value);

    let settings = new BrightnessProcessorSettings();
    settings.brightness = this.brightnessProcessor.brightness;
    this.lightStripService.setBrightnessSettings(settings);
  }

  onMaxBrightnessChange(event: any) {    
    this.brightnessProcessor.maxBrightness = parseFloat(event.target.value);

    let settings = new BrightnessProcessorSettings();
    settings.maxBrightness = this.brightnessProcessor.maxBrightness;
    this.lightStripService.setBrightnessSettings(settings);
  }

  onGammaRChange(event: any) {    
    this.brightnessProcessor.gamma.x = parseFloat(event.target.value);

    let settings = new BrightnessProcessorSettings();
    settings.gamma = this.brightnessProcessor.gamma;
    this.lightStripService.setBrightnessSettings(settings);
  }

  onGammaGChange(event: any) {    
    this.brightnessProcessor.gamma.y = parseFloat(event.target.value);

    let settings = new BrightnessProcessorSettings();
    settings.gamma = this.brightnessProcessor.gamma;
    this.lightStripService.setBrightnessSettings(settings);
  }

  onGammaBChange(event: any) {    
    this.brightnessProcessor.gamma.z = parseFloat(event.target.value);

    let settings = new BrightnessProcessorSettings();
    settings.gamma = this.brightnessProcessor.gamma;
    this.lightStripService.setBrightnessSettings(settings);
  }

  onLightCountChange(event: any) {    
    this.device.lightCount = parseFloat(event.target.value);

    let settings = new DeviceSettings();
    settings.lightCount = this.device.lightCount;
    this.lightStripService.setDeviceSettings(settings);
  }
}
