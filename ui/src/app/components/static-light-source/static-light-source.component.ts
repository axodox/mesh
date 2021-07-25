import { Component, Input, OnInit, ChangeDetectionStrategy, ChangeDetectorRef } from '@angular/core';
import { AnyLightSourceSettings, StaticSourceSettings } from 'src/app/data/light-strip-settings';
import { LightStripService } from 'src/app/services/light-strip-service';

@Component({
  selector: 'app-static-light-source',
  templateUrl: './static-light-source.component.html',
  styleUrls: ['./static-light-source.component.scss'],
  changeDetection: ChangeDetectionStrategy.OnPush
})
export class StaticLightSourceComponent implements OnInit {

  @Input() source: AnyLightSourceSettings; 

  constructor(
    private readonly changeDetector: ChangeDetectorRef,
    private readonly lightStripService : LightStripService
  ) { }

  ngOnInit(): void {
  }

  async onColorChange(event: any) {
    let sourceSettings = new StaticSourceSettings();
    sourceSettings.color = event.target.value;
    await this.lightStripService.setLightSourceSettings(sourceSettings).toPromise();
    this.changeDetector.detectChanges();
  }
}
