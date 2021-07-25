import { ChangeDetectionStrategy, ChangeDetectorRef, Component, OnInit } from '@angular/core';
import { AnyLightSourceSettings, NoneSourceSettings } from 'src/app/data/light-strip-settings';
import { LightStripService } from 'src/app/services/light-strip-service';

@Component({
  selector: 'app-light-strip',
  templateUrl: './light-strip.component.html',
  styleUrls: ['./light-strip.component.scss'],
  changeDetection: ChangeDetectionStrategy.OnPush
})
export class LightStripComponent implements OnInit {

  source: AnyLightSourceSettings;

  getType() {
    return this.source?.$type ?? "none";
  }

  constructor(
    private readonly changeDetector: ChangeDetectorRef,
    private readonly lightStripService : LightStripService
  ) { }

  async ngOnInit() {
    this.source = await this.lightStripService.getLightSourceSettings().toPromise();
    this.changeDetector.detectChanges();
  }

  async onSourceChange(event: any) {
    let sourceSettings = new NoneSourceSettings();
    sourceSettings.$type = event.target.value;
    await this.lightStripService.setLightSourceSettings(sourceSettings).toPromise();
    this.source = await this.lightStripService.getLightSourceSettings().toPromise();
    this.changeDetector.detectChanges();
  }
}
