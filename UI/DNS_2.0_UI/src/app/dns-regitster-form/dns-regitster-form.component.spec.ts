import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DNSRegitsterFormComponent } from './dns-regitster-form.component';

describe('DNSRegitsterFormComponent', () => {
  let component: DNSRegitsterFormComponent;
  let fixture: ComponentFixture<DNSRegitsterFormComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ DNSRegitsterFormComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DNSRegitsterFormComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
