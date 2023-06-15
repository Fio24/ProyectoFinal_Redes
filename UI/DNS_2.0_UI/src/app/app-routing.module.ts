import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { HomeComponent } from './home/home.component';
import { DNSRegitsterFormComponent } from './dns-regitster-form/dns-regitster-form.component';

const routes: Routes = [
  {path: "", component:HomeComponent},
  {path: "new", component:DNSRegitsterFormComponent}
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
