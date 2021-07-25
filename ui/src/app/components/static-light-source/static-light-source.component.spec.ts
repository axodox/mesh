import { ComponentFixture, TestBed } from '@angular/core/testing';

import { StaticLightSourceComponent } from './static-light-source.component';

describe('StaticLightSourceComponent', () => {
  let component: StaticLightSourceComponent;
  let fixture: ComponentFixture<StaticLightSourceComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ StaticLightSourceComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(StaticLightSourceComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
