import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-dns-edit-form',
  templateUrl: './dns-edit-form.component.html',
  styleUrls: ['./dns-edit-form.component.css']
})
export class DnsEditFormComponent implements OnInit {

  constructor() { }

  servers:string[] = []
  serversWeight:[string, number][] = []
  type:string = "single";

  ngOnInit(): void {
    
  }

  removeServer(index:number){
    this.servers.splice(index,1);
  }

  addServer(){
    this.servers.push("0.0.0.0")
  }

  removeServerWeight(index:number){
    this.serversWeight.splice(index,1);
  }

  addServerWeight(){
    this.serversWeight.push(["0.0.0.0", 0])
  }

}
