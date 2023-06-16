import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable, take } from 'rxjs';
import { environment } from 'src/environments/environment';

@Injectable({
  providedIn: 'root'
})
export class DnsService {

  constructor(private httpClient: HttpClient) { }

  getAll(){
    return this.httpClient
    .get("/api/getAll")
    .pipe(take(1))
  }

  new( register:any){
    return this.httpClient
    .put("/api/new", register)
    .pipe(take(1))
  }

  delete(url:string){
    const options = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json',
      }),
      body: {
        "url":url
      },
    };
    return this.httpClient
    .delete("/api/del",options )
    .pipe(take(1))
  }

  getById(id:string){
    return this.httpClient
    .get(`/api/get/${id}`)
    .pipe(take(1))
  }

  update( register:any){
    return this.httpClient
    .post("/api/update", register)
    .pipe(take(1))
  }
}


