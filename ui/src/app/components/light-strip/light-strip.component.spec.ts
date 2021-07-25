import { ComponentFixture, TestBed } from '@angular/core/testing';

import { LightStripComponent } from './light-strip.component';

describe('LightStripComponent', () => {
  let component: LightStripComponent;
  let fixture: ComponentFixture<LightStripComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ LightStripComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(LightStripComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
