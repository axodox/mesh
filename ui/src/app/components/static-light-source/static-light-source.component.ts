import { Component, Input, ChangeDetectionStrategy } from '@angular/core';
import { AnyLightSourceSettings, StaticSourceSettings } from 'src/app/data/light-strip-settings';
import { LightStripService } from 'src/app/services/light-strip-service';

@Component({
  selector: 'app-static-light-source',
  templateUrl: './static-light-source.component.html',
  styleUrls: ['./static-light-source.component.scss'],
  changeDetection: ChangeDetectionStrategy.OnPush
})
export class StaticLightSourceComponent {

  @Input() source: AnyLightSourceSettings; 

  constructor(
    private readonly lightStripService : LightStripService
  ) { }

  onColorChange(event: any) {
    let sourceSettings = new StaticSourceSettings();
    sourceSettings.color = event.target.value;
    this.lightStripService.setLightSourceSettings(sourceSettings);
  }
}
