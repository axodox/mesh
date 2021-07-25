import { ChangeDetectionStrategy, ChangeDetectorRef, Component, Input } from '@angular/core';
import { AnyLightSourceSettings } from 'src/app/data/light-strip-settings';
import { LightStripService } from 'src/app/services/light-strip-service';

@Component({
  selector: 'app-rainbow-light-source',
  templateUrl: './rainbow-light-source.component.html',
  styleUrls: ['./rainbow-light-source.component.scss'],
  changeDetection: ChangeDetectionStrategy.OnPush
})
export class RainbowLightSourceComponent {

  @Input() source: AnyLightSourceSettings; 

  constructor(
    private readonly changeDetector: ChangeDetectorRef,
    private readonly lightStripService : LightStripService
  ) { }

  async onSpatialFrequencyChange(event: any) {
    this.source.spatialFrequency = parseFloat(event.target.value);
    await this.applyChanges();
  }

  async onAngularVelocityChange(event: any) {
    this.source.angularVelocity = parseFloat(event.target.value);
    await this.applyChanges();
  }

  private async applyChanges() {
    await this.lightStripService.setLightSourceSettings(this.source).toPromise();
  }
}
